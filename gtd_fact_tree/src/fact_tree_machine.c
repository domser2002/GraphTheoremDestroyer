#include "fact_tree_machine.h"

typedef enum MachineState 
{
    INITIATED = 0,
    EXECUTING,
    EXECUTED,
    WRITING,
    WRITTEN
} MachineState;
struct FactTreeMachine
{
    FactTree *FactTree;
    bool contradictionFound;
    uint32_t *contradiciting_idxs;
    uint32_t contradicting_count;
    uint64_t checked;
    MachineState state;
};

/**
 * \brief initialize new proof machine based on initial fact tree
 * \param FactTree initial fact tree
 * \returns newly created machine
*/
FactTreeMachine *init_machine(FactTree *FactTree)
{
    GTD_LOG("Initiating proof machine");
    FactTreeMachine *newMachine = (FactTreeMachine *)gtd_malloc(sizeof(FactTreeMachine));
    newMachine->FactTree = FactTree;
    newMachine->contradictionFound = false;
    newMachine->contradiciting_idxs = NULL;
    newMachine->contradicting_count = 0;
    newMachine->checked = 0;
    newMachine->state = INITIATED;
    return newMachine;
}

/**
 * \brief delete proof machine
 * \param machine machine to delete
*/
void delete_machine(FactTreeMachine *machine)
{
    GTD_LOG("Deleting proof machine");
    if (machine->contradiciting_idxs != NULL)
        gtd_free(machine->contradiciting_idxs);
    gtd_free(machine);
}

/**
 * \brief perform proof machine execution
 * \details machine tries to find predefined contradiciton in the given tree of facts, 
 * if it is not found, it looks for one of the predefined implications, adds new facts to a tree
 * and executes recursively until contradiction is found or no more facts can be added 
 * \param machine machine to be executed
 * \note function does not return, it only updates fields in machine class, proof can be written
 *  using write_proof function
*/
void execute(FactTreeMachine *machine)
{
    GTD_LOG("Executing proof machine");
    machine->state = EXECUTING;
    uint32_t n = machine->FactTree->fact_count;
    // find contradiction
    uint64_t max_number = 1ULL << n;
    for (uint64_t mask = machine->checked; mask < max_number; mask++)
    {
        Fact **subArray = (Fact **)gtd_malloc(MAX_CONTRADICTING_FACTS * sizeof(Fact *));
        uint8_t elements[MAX_CONTRADICTING_FACTS];
        uint8_t number_of_ones = 0;
        for (uint8_t i = 0; i < n && number_of_ones < MAX_CONTRADICTING_FACTS; i++)
        {
            if (mask & (1ULL << i))
            {
                elements[number_of_ones] = n - i - 1;
                subArray[number_of_ones++] = machine->FactTree->facts[n - i - 1];
            }
        }
        if (number_of_ones >= MIN_CONTRADICTING_FACTS && number_of_ones <= MAX_CONTRADICTING_FACTS && contradict(subArray, number_of_ones))
        {
            GTD_LOG("Contradiction found at subarray (hex format): %x", mask);
            machine->contradictionFound = true;
            machine->contradiciting_idxs = (uint32_t *)gtd_malloc(number_of_ones * sizeof(uint32_t));
            for (uint8_t i = 0; i < number_of_ones; i++)
                machine->contradiciting_idxs[i] = elements[i];
            machine->contradicting_count = number_of_ones;
            machine->state = EXECUTED;
            gtd_free(subArray);
            return;
        }
        GTD_LOG("Contradiction not found at subarray (hex format): %x", mask);
        gtd_free(subArray);
    }
    // add facts
    bool added_facts = false;
    for (uint64_t mask = machine->checked; mask < max_number; mask++)
    {
        Fact **subArray = (Fact **)gtd_malloc(MAX_LEFT_SIDE_FACTS * sizeof(Fact *));
        uint8_t elements[MAX_LEFT_SIDE_FACTS];
        uint8_t number_of_ones = 0;
        for (uint8_t i = 0; i < n && number_of_ones < MAX_LEFT_SIDE_FACTS; i++)
        {
            if (mask & (1ULL << i))
            {
                elements[number_of_ones] = n - i - 1;
                subArray[number_of_ones++] = machine->FactTree->facts[n - i - 1];
            }
        }
        if (number_of_ones >= MIN_LEFT_SIDE_FACTS && number_of_ones <= MAX_LEFT_SIDE_FACTS)
        {
            int count;
            Fact **newFacts = implies(subArray, number_of_ones, &count);
            for (int i = 0; i < count; i++)
            {
                uint32_t *parents = (uint32_t *)gtd_malloc(number_of_ones * sizeof(uint32_t));
                for (uint8_t j = 0; j < number_of_ones; j++)
                {
                    parents[j] = elements[j];
                }
                add_fact(machine->FactTree, parents, number_of_ones, newFacts[i]);
                added_facts = true;
            }
            GTD_LOG("Implication found at subarray (hex format): %x", mask);
            gtd_free(subArray);
            continue;
        }
        GTD_LOG("Implication not found at subarray (hex format): %x", mask);
        gtd_free(subArray);
    }
    if (added_facts)
    {
        machine->checked = max_number;
        execute(machine);
    }
    machine->state = EXECUTED;
}

static void write_deduction(FactTree *FactTree, uint32_t idx, FILE *output)
{
    uint8_t parent_count = FactTree->parent_count[idx];
    uint32_t *parents = FactTree->parents[idx];
    for (uint8_t i = 0; i < parent_count; i++)
    {
        write_deduction(FactTree, parents[i], output);
        fprintf(output,"\n");
    }
    for (uint8_t i = 0; i < parent_count; i++)
    {
        fprintf(output, "%s",get_fact_str(FactTree->facts[parents[i]]));
        if(i != parent_count - 1) fprintf(output,", ");
    }
    if(parent_count != 0) fprintf(output, " => ");
    char *str = get_fact_str((Fact *)FactTree->facts[idx]);
    fprintf(output, "%s", (const char *)str);
    gtd_free((void *)str);
}

bool write_proof(FactTreeMachine *machine, FILE *output)
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
        for(uint32_t i=0;i<machine->contradicting_count;i++)
        {
            fprintf(output, "%s",get_fact_str(machine->FactTree->facts[machine->contradiciting_idxs[i]]));
            if(i != machine->contradicting_count - 1) fprintf(output,", ");
        }
        fprintf(output," contradict\n");
    }
    machine->state = WRITTEN;
    return true;
}