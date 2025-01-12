#define IS_FACT_TREE_COMPONENT
#include "fact_tree.h"
#include "common.h"
#include <stdlib.h>

FactTree *construct(uint32_t fact_count, Fact **facts)
{
    GTD_LOG("Constructing FactTree with %d facts", fact_count);
    FactTree* ft = (FactTree*)gtd_malloc(sizeof(FactTree));
    ft->fact_count = fact_count;
    ft->facts = facts;
    ft->parents = (uint32_t**)gtd_malloc(fact_count*sizeof(uint32_t*));
    ft->parent_count = (uint8_t*)gtd_malloc(fact_count*sizeof(uint8_t));
    for(uint32_t i=0;i<fact_count;i++)
    {
        ft->parent_count[i] = 0;
        ft->parents[i] = NULL;
    }
    return ft;
}

void destruct(FactTree* ft)
{
    GTD_LOG("Destructing FactTree with %d facts", ft->fact_count);
    for(uint32_t i=0;i<ft->fact_count;i++)
    {
        gtd_free(ft->parents[i]);
    }
    gtd_free(ft->parents);
    gtd_free(ft->parent_count);
    gtd_free(ft);
}

static bool exists(FactTree *ft,Fact *new_fact)
{
    for(uint32_t i=0;i<ft->fact_count;i++)
    {
        if(equal_facts(ft->facts[i],new_fact))
            return true;
    }
    return false;
}

bool add_fact(FactTree *ft, uint32_t *parent_idxs, uint8_t parent_count, Fact *new_fact)
{
    char *fact_str = get_fact_str(new_fact);
    GTD_LOG("Adding new fact: %s, with parent_idx = %d to a FactTree", fact_str, parent_idxs[0]);
    free(fact_str);
    if(exists(ft,new_fact)) 
    {
        GTD_LOG("Fact already exists");
        return false;
    }
    ft->fact_count++;
    ft->facts = (Fact**)gtd_realloc(ft->facts,ft->fact_count * sizeof(Fact*));
    ft->facts[ft->fact_count-1] = new_fact;
    ft->parents = (uint32_t**)gtd_realloc(ft->parents,ft->fact_count*sizeof(uint32_t*));
    ft->parents[ft->fact_count-1] = parent_idxs;
    ft->parent_count = (uint8_t*)gtd_realloc(ft->parent_count,ft->fact_count*sizeof(uint8_t));
    ft->parent_count[ft->fact_count-1] = parent_count;
    return true;
}
