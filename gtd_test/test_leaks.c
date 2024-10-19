#include <stdio.h>
#include "graph.h"

void run_all_functions()
{
    FactTree* g = construct_graph(10);
    destruct_graph(g);
    return;
}

int main(int argc,char**argv)
{
    run_all_functions();
    return 0;
}