#define IS_GENERATIVE_MODULE_COMPONENT
#include "generative_main.h"
#include "generative_proof_machine.h"
#include "generative_restriction.h"
#define MAX_VERTICES 100

void *generative_main_loop(void *argument)
{
    GTD_LOG("Entered generative module main loop");
    ModuleArgs *args = (ModuleArgs *)argument;
    GenerativeProofMachine *machine;
    Fact **used_facts = (Fact **)gtd_malloc(args->fact_count * sizeof(Fact **));
    uint32_t used_facts_count = 0;
    Graph *start_graph = create_graph(MAX_VERTICES, 0);
    machine = create_proof_machine(start_graph);
    for(uint32_t i=0;i<args->fact_count;i++)
    {
        GenerativeRestriction *restriction = create_restriction_from_fact(args->fact_array[i], machine);
        if(restriction)
            used_facts[used_facts_count++] = args->fact_array[i];
        add_restriction(machine, restriction);
    }
    used_facts = (Fact **)gtd_realloc(used_facts, used_facts_count * sizeof(Fact *));
    while(true)
    {
        bool reexecute_machine = false;
        uint8_t found = execute_generative_proof_machine(machine);
        MessageStruct *msg = (MessageStruct*)gtd_malloc(sizeof(MessageStruct));
        if(found)
        {
            msg->type = ContradictionFoundMessage;
            ContradictionFoundMessageBody *body = (ContradictionFoundMessageBody *)gtd_malloc(sizeof(ContradictionFoundMessageBody));
            body->contradicting_facts = used_facts;
            body->contradicting_facts_number = used_facts_count;
        }
        else
        {
            msg->type = NothingFoundMessage;
            msg->body = NULL;
        }
        if(write(args->write_fd, (const void *)msg, sizeof(MessageStruct)) == -1)
        {
            GTD_LOG("Error in write");
            exit(EXIT_FAILURE);
        }
        GTD_LOG("Generative module sent a message of type %d, waiting for response", msg->type);
        while(true)
        {
            bool wait_for_another_response = false;
            MessageStruct response;
            if(read(args->read_fd, &response, sizeof(MessageStruct)) == -1)
            {
                GTD_LOG("error in read");
                exit(EXIT_FAILURE);
            }
            GTD_LOG("Generative module got response of type %d", response.type);
            switch (response.type)
            {
            case WriteProofMessage:
                write_proof_tree(get_machine_proof_tree(machine), stdout);
                break;
            case WriteDeductionMessage:
                write_proof_tree(get_machine_proof_tree(machine), stdout);
                break;
            case AddFactsMessage:
                wait_for_another_response = true;
                reexecute_machine = true;
                AddFactsMessageBody *add_facts_msg_body = (AddFactsMessageBody*)response.body;
                used_facts = (Fact **)gtd_realloc(used_facts, (used_facts_count + add_facts_msg_body->fact_count) * sizeof(Fact *));
                for(uint32_t i=0; i<add_facts_msg_body->fact_count;i++)
                {
                    GenerativeRestriction *restriction = create_restriction_from_fact(add_facts_msg_body->fact_array[i], machine);
                    if(restriction)
                        used_facts[used_facts_count++] = args->fact_array[i];
                    add_restriction(machine, restriction);
                }
                break;
            case StopModuleMessage:
                wait_for_another_response = false;
                reexecute_machine = false;
                break;
            default:
                GTD_LOG("Received invalid message");
                exit(EXIT_FAILURE);
            }
            if(!wait_for_another_response)
                break;
        }
        if(!reexecute_machine)
            break;
    }
    free_module_args(args);
    GTD_LOG("Generative module terminating");
    return NULL;
}