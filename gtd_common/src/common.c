#include "common.h"

void *gtd_malloc(size_t size)
{
    void *ptr = malloc(size);
    return ptr;
}

void *gtd_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb,size);
    return ptr;
}

void *gtd_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    return ptr;
}

void gtd_free(void *ptr)
{
    return free(ptr);
}

void printProgressBar(int progress, int total) {
    int barWidth = 70; // Width of the progress bar
    float percentage = (float)progress / total;

    printf("\033[0G["); // Move cursor to the beginning of the line and print the start of the bar
    int pos = barWidth * percentage;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) 
            printf("=");
        else 
            printf(" ");
    }
    printf("] %.2f%%", (double)percentage * 100); // Display percentage
    fflush(stdout); // Flush the output buffer to update the display
}

/**
 * \brief function to check how many parameters are required for fact type
 * \param type fact type
 * \return number of parameters needed
*/
uint8_t get_param_count(FactType type)
{
    switch (type)
    {
    case IstnaryTreeFact:
    case IsPartiteFact:
    case VertexCountFact:
    case MinVertexCountFact:
    case MaxVertexCountFact:
    case EdgeCountFact:
    case MinEdgeCountFact:
    case MaxEdgeCountFact:
    case TreeHeightFact:
    case MinTreeHeightFact:
    case MaxTreeHeightFact:
    case HasCycleFact:
    case HasNoCycleFact:
    case HasInducedCycleFact:
    case HasNoInducedCycleFact:
    case HasMinorCycleFact:
    case HasNoMinorCycleFact:
    case HasPathFact:
    case HasNoPathFact:
    case HasInducedPathFact:
    case HasNoInducedPathFact:
    case HasMinorPathFact:
    case HasNoMinorPathFact:
    case HasCliqueFact:
    case HasNoCliqueFact:
    case HasMinorCliqueFact:
    case HasNoMinorCliqueFact:
    case MaxVertexDegreeFact:
    case MinVertexDegreeFact:
    case HasNoUnknownEdgesFact:
        return 1;
    case IsConnectedFact:
    case IsTreeFact:
    case IsPlanarFact:
    case IsCycleFact:
    case IsCycleComplementFact:
    case HasNoCyclesFact:
        return 0;
    case HasCompletePartiteFact:
    case HasNoCompletePartiteFact:
    case HasInducedCompletePartiteFact:
    case HasNoInducedCompletePartiteFact:
    case HasMinorCompletePartiteFact:
    case HasNoMinorCompletePartiteFact:
        // handle only bipartite for now
        return 3;
    default:
        return 0;
    }
}