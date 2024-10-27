#include "fact_tree.h"
#include "common.h"
#include <stdlib.h>

/**
 * \brief function to create an initial FactTree with isolated facts
 * \param fact_count number of facts
 * \param facts array of pointers to facts
 * \returns pointer to a newly created FactTree
*/
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

/**
 * \brief function to destruct FactTree
 * \param ft FactTree to destruct
*/
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

/**
 * \brief function to check if fact with the same set of parents already exists in fact tree 
 * \note used temporary to avoid infinite adding of same implication, machine will handle it in future
 * \param ft fact tree
 * \param parent_idxs array of parent indexes
 * \param parent_count number of parents
 * \returns true if fact with the same set of parents already exists, false otherwise
*/
static bool exists(FactTree *ft,uint32_t *parent_idxs, uint8_t parent_count)
{
    for(uint32_t i=0;i<ft->fact_count;i++)
    {
        bool all_equal = true;
        if(ft->parent_count[i] != parent_count) continue; 
        for(uint8_t j=0;j<ft->parent_count[i];j++)
        {
            if(ft->parents[i][j] != parent_idxs[j])
            {
                all_equal = false;
                break;
            }
        }
        if(all_equal)
            return true;
    }
    return false;
}

/**
 * \brief function to add new fact to the FactTree, unless it already exists
 * \param ft - fact tree
 * \param parent_idxs - array of parents of the new Fact
 * \param parent_count - number of parents of the new Fact
 * \param new_fact - fact to be added
*/
bool add_fact(FactTree *ft, uint32_t *parent_idxs, uint8_t parent_count, Fact *new_fact)
{
    GTD_LOG("Adding new fact with parent_idx = %d to a FactTree", parent_idxs[0]);
    if(exists(ft,parent_idxs,parent_count)) 
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
