#include "machine.h"

typedef enum MachineState 
{
    INITIATED = 0,
    EXECUTING,
    EXECUTED,
    WRITING,
    WRITTEN
} MachineState;
struct ProofMachine
{
    Graph *FactTree;
    bool contradictionFound;
    uint32_t *contradiciting_idxs;
    uint32_t contradicting_count;
    MachineState state;
};

ProofMachine *init_machine(Graph *FactTree)
{
    ProofMachine *newMachine = (ProofMachine *)gtd_malloc(sizeof(ProofMachine));
    newMachine->FactTree = FactTree;
    newMachine->contradictionFound = false;
    newMachine->contradiciting_idxs = NULL;
    newMachine->contradicting_count = 0;
    newMachine->state = INITIATED;
    return newMachine;
}

void delete_machine(ProofMachine *machine)
{
    if (machine->contradiciting_idxs != NULL)
        gtd_free(machine->contradiciting_idxs);
    gtd_free(machine);
}

void execute(ProofMachine *machine)
{
    machine->state = EXECUTING;
    uint32_t n = machine->FactTree->vertexCount;
    // find contradiction
    for (uint32_t i = 0; i < n; i++)
    {
        for (uint32_t j = i; j < n; j++)
        {
            Fact *fact1 = (Fact *)machine->FactTree->vertexData[i];
            Fact *fact2 = (Fact *)machine->FactTree->vertexData[j];
            if(contradict(fact1,fact2))
            {
                machine->contradictionFound = true;
                machine->contradiciting_idxs = (uint32_t*)gtd_malloc(2 * sizeof(uint32_t));
                machine->contradiciting_idxs[0] = i;
                machine->contradiciting_idxs[1] = j;
                machine->contradicting_count = 2;
                machine->state = EXECUTED;
                return;
            }
        }
    }
    // add facts
    for(uint32_t i = 0; i < n; i++)
    {
        Fact *newFact = results((Fact *)machine->FactTree->vertexData[i]);
        if(newFact != NULL)
        {
            if(add_vertex_with_edge(machine->FactTree,i,(void*)newFact))
                execute(machine);
            else
                gtd_free(newFact);
        }
    }
    machine->state = EXECUTED;
    // generate cases
}

static void write_deduction(Graph *FactTree, uint32_t idx, FILE *output)
{
    int parent = get_parent(FactTree,idx);
    if(parent != -1)
    {
        fprintf(output," => ");
        write_deduction(FactTree,idx,output);
    }
    char *str = get_fact_str((Fact*)FactTree->vertexData[idx]);
    fprintf(output,"%s",(const char *)str);
    gtd_free((void*)str);
}

bool write_proof(ProofMachine *machine, FILE *output)
{
    if(machine->state != EXECUTED)
    {
        GTD_LOG("[ERROR] Wrong machine state!");
        return false;
    }
    machine->state = WRITING;
    if(!machine->contradictionFound)
    {
        fprintf(output,"Contradiction not found.\n");
    }
    else
    {
        for(uint32_t i=0;i<machine->contradicting_count;i++)
        {
            write_deduction(machine->FactTree,machine->contradiciting_idxs[i],output);
            fprintf(output,"\n");
        }
    }
    machine->state = WRITTEN;
    return true;
}