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
    FactTree *FactTree;
    bool contradictionFound;
    uint32_t *contradiciting_idxs;
    uint32_t contradicting_count;
    MachineState state;
};

ProofMachine *init_machine(FactTree *FactTree)
{
    GTD_LOG("Initiating proof machine");
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
    GTD_LOG("Deleting proof machine");
    if (machine->contradiciting_idxs != NULL)
        gtd_free(machine->contradiciting_idxs);
    gtd_free(machine);
}

void execute(ProofMachine *machine)
{
    GTD_LOG("Executing proof machine");
    machine->state = EXECUTING;
    uint32_t n = machine->FactTree->fact_count;
    // find contradiction
    for (uint32_t i = 0; i < n; i++)
    {
        for (uint32_t j = i+1; j < n; j++)
        {
            Fact **factArray = (Fact**)gtd_malloc(2*sizeof(Fact*));
            factArray[0] = (Fact *)machine->FactTree->facts[i];
            factArray[1] = (Fact *)machine->FactTree->facts[j];
            if(contradict(factArray,2))
            {
                GTD_LOG("Contradiction found at indexes: %d %d",i,j);
                machine->contradictionFound = true;
                machine->contradiciting_idxs = (uint32_t*)gtd_malloc(2 * sizeof(uint32_t));
                machine->contradiciting_idxs[0] = i;
                machine->contradiciting_idxs[1] = j;
                machine->contradicting_count = 2;
                machine->state = EXECUTED;
                gtd_free(factArray);
                return;
            }
            GTD_LOG("Contradiction not found at indexes: %d %d",i,j);
            gtd_free(factArray);
        }
    }
    // add facts
    for(uint32_t i = 0; i < n; i++)
    {
        Fact **factArray = (Fact **)gtd_malloc(sizeof(Fact*));
        factArray[0] = (Fact*)machine->FactTree->facts[i];
        int count;
        bool execute_machine = true;
        Fact **newFacts = implies(factArray, 1, &count);
        for(int j=0;j<count;j++)
        {
            GTD_LOG("%s implies %s",get_fact_str(factArray[0]), get_fact_str(newFacts[j]));
            uint32_t *parents = (uint32_t*)gtd_malloc(1 * sizeof(uint32_t));
            parents[0] = i;
            if(!add_fact(machine->FactTree,parents,1,newFacts[j]))
                execute_machine = false;
        }
        gtd_free(newFacts);
        if(execute_machine && count > 0)
            execute(machine);
        // if(newFacts != NULL && count != 0)
        // {
        //     if(add_vertex_with_edge(machine->FactTree,i,(void*)newFacts[0]))
        //         execute(machine);
        //     else
        //         gtd_free(newFacts);
        // }
    }
    machine->state = EXECUTED;
    // generate cases
}

static void write_deduction(FactTree *FactTree, uint32_t idx, FILE *output)
{
    uint8_t parent_count = FactTree->parent_count[idx];
    uint32_t *parents = FactTree->parents[idx];
    //int parent = get_parent(FactTree,idx);
    for(uint8_t i=0;i<parent_count;i++)
    {
        fprintf(output," => ");
        write_deduction(FactTree,parents[i],output);
    }
    char *str = get_fact_str((Fact*)FactTree->facts[idx]);
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