#define IS_FACT_TREE_COMPONENT
#include "fact_tree_main.h"
#include "fact_tree.h"
#include "fact_tree_machine.h"

void *fact_tree_main_loop(void *argument)
{
    GTD_LOG("Entered fact tree module main loop");
    ModuleArgs *args = (ModuleArgs*)argument;
    FactTree *fact_tree = construct(args->fact_count, args->fact_array);
    FactTreeMachine *machine = init_machine(fact_tree);
    while(true)
    {
        bool reexecute_machine = false;
        execute(machine);
        bool contradiciton_found = get_contradiction_found(machine);
        MessageStruct *msg = (MessageStruct*)gtd_malloc(sizeof(MessageStruct));
        if(contradiciton_found)
        {
            msg->type = ContradictionFoundMessage;
            ContradictionFoundMessageBody* body = (ContradictionFoundMessageBody*)gtd_malloc(sizeof(ContradictionFoundMessageBody));
            body->contradicting_facts = get_contradicting_facts(machine, &body->contradicting_facts_number);
            msg->body = body;
        }
        else
        {
            FactTree *fact_tree = get_fact_tree(machine);
            if(fact_tree->fact_count > args->fact_count)
            {
                msg->type = FactsFoundMessage;
                FactsFoundMessageBody *body = (FactsFoundMessageBody*)gtd_malloc(sizeof(FactsFoundMessageBody));
                body->facts_found_number = fact_tree->fact_count - args->fact_count;
                body->facts_found = (fact_tree->facts + body->facts_found_number * sizeof(Fact*));
                msg->body = body;
            }
            else
            {
                msg->type = NothingFoundMessage;
                msg->body = NULL;
            }
        }
        if(write(args->write_fd, (const void *)msg, sizeof(MessageStruct)) == -1)
        {
            GTD_LOG("Error in write");
            exit(EXIT_FAILURE);
        }
        GTD_LOG("Fact tree module sent a message of type %d, waiting for response", msg->type);
        while(true)
        {
            bool wait_for_another_response = false;
            MessageStruct response;
            if(read(args->read_fd, &response, sizeof(MessageStruct)) == -1)
            {
                GTD_LOG("error in read");
                exit(EXIT_FAILURE);
            }
            GTD_LOG("Fact tree module got response of type %d", response.type);
            FactTree *ft = get_fact_tree(machine);
            switch (response.type)
            {
            case WriteProofMessage:
                FILE *out_file = fopen(args->out_file_path, "a");
                write_proof(machine, out_file);
                fclose(out_file);
                break;
            case WriteDeductionMessage:
                wait_for_another_response = true;
                WriteDeductionMessageBody *write_deduction_msg_body = (WriteDeductionMessageBody*)response.body;
                for(uint32_t i=0;i<ft->fact_count;i++)
                {
                    if(equal_facts(ft->facts[i], write_deduction_msg_body->fact))
                    {
                        FILE *out_file = fopen(args->out_file_path, "a");
                        write_deduction(ft, i, stdout);
                        fclose(out_file);
                    }
                }
                break;
            case AddFactsMessage:
                wait_for_another_response = true;
                reexecute_machine = true;
                AddFactsMessageBody *add_facts_msg_body = (AddFactsMessageBody*)response.body;
                for(uint32_t i=0; i<add_facts_msg_body->fact_count;i++)
                {
                    add_fact(ft,NULL,0,add_facts_msg_body->fact_array[i]);
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
    delete_machine(machine);
    destruct(fact_tree);
    GTD_LOG("Fact tree module terminating");
    return NULL;
}