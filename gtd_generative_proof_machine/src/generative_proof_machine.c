#include "generative_proof_machine.h"
#include "physical_graph.h"
#include "generative_restriction.h"
/*
#include "common.h"
#include "physical_graph.h"
#include "generative_restriction.h"
*/

typedef struct GraphNode
{
    Graph *graph;
    struct GraphNode *next;
}GraphNode;

struct GenerativeProofMachine
{
    GraphNode *graphs; //head
    GenerativeRestriction **restrictions;
    int num_restrictions;
};

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions, int num_restrictions)
{
    GenerativeProofMachine *generativeProofMachine = gtd_malloc(sizeof(GenerativeProofMachine));
    generativeProofMachine->graphs = NULL;
    generativeProofMachine->restrictions = restrictions;
    generativeProofMachine->num_restrictions = num_restrictions;

    return generativeProofMachine;
}

int destroy_generative_proof_machine(GenerativeProofMachine *machine)
{
    clear_graphs(machine, 1);
    for(int i = 0; i < machine->num_restrictions; ++i)
    {
        destroy_restriction(machine->restrictions[i]);
    }
    gtd_free(machine);

    return 1;
}

void add_graph(GenerativeProofMachine *machine, Graph *graph)
{
    if(machine->graphs == NULL)
    {
        machine->graphs = gtd_malloc(sizeof(GraphNode));
        machine->graphs->next = NULL;
    }
    else
    {
        GraphNode *oldList = machine->graphs;
        machine->graphs = gtd_malloc(sizeof(GraphNode));
        machine->graphs->next = oldList;
    }
    machine->graphs->graph = graph;
}

void clear_graphs(GenerativeProofMachine *machine, int delete)
{
    if(delete)
    {
        GraphNode *graphNode = machine->graphs;
        while(graphNode != NULL)
        {
            GraphNode *nextNode = graphNode->next;
            destroy_graph(graphNode->graph);
            gtd_free(graphNode);
            graphNode = nextNode;
        }
    }
    machine->graphs = NULL;
}

int can_add_graph(GenerativeProofMachine *machine, Graph *graph)
{
    for(int j = 0; j < machine->num_restrictions; ++j)
    {
        if(!check_restriction(graph, machine->restrictions[j]))
        {
            return 0;
        }
    }

    GraphNode *graphNode = machine->graphs;
    while(graphNode != NULL)
    {
        if(check_isomorphic(graphNode->graph, graph))
        {
            return 0;
        }
        graphNode = graphNode->next;
    }
    return 1;
}

int execute_generative_proof_machine(GenerativeProofMachine *machine)
{
    if(machine->graphs == NULL || machine->num_restrictions == 0)
    {
        return 0;
    }

    GraphNode *oldGraphs = NULL;
    while(1)
    {
        oldGraphs = machine->graphs;
        clear_graphs(machine, 0);

        GraphNode *node = oldGraphs;
        while(node != NULL)
        {
            Graph *graph = node->graph;
            int graph_vertices_num = get_graph_num_vertices(graph);
            int new_graphs_num = int_pow(2, graph_vertices_num);
            for(int i = 0; i < new_graphs_num; ++i)
            {
                Graph *newGraph = copyGraph(graph);
                add_vertex(newGraph);
                int j = i;
                int count = 0;
                while(j > 0)
                {
                    if(j & 1)
                    {
                        add_edge(newGraph, graph_vertices_num, count);
                    }
                    ++count;
                    j = j >> 1;
                }

                if(can_add_graph(machine, newGraph))
                {
                    add_graph(machine, newGraph);
                }
            }

            node = node->next;
        }

        if(machine->graphs == NULL)
        {
            break;
        }
    }

    FILE *out_file = fopen("matrices.txt", "w");
    while(oldGraphs != NULL)
    {
        Graph *g = oldGraphs->graph;
        int n = get_graph_num_vertices(g);
        int **am = get_graph_adjacency_matrix(g);
        for(int r = 0; r < n; ++r)
        {
            for(int c = 0; c < n; ++c)
            {
                fprintf(out_file, "%d ", am[r][c]);
            }
            fprintf(out_file, "\n");
        }
        fprintf(out_file, "\n");
        oldGraphs = oldGraphs->next;
    }
    fclose(out_file);

    return 1;
}

int int_pow(int base, int exp)
{
    int res = 1;
    for(int i = 0; i < exp; ++i)
    {
        res *= base;
    }
    return res;
}