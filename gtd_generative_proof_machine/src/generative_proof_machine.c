#include "generative_proof_machine.h"
#include "physical_graph.h"
#include "generative_restriction.h"

typedef struct GraphNode
{
    Graph *graph;
    struct GraphNode *next;
}GraphNode;

struct GenerativeProofMachine
{
    int num_hash_buckets;
    GraphNode **graph_buckets;
    GenerativeRestriction **restrictions;
    int num_restrictions;
    int num_graphs;
};

GenerativeProofMachine *create_generative_proof_machine(GenerativeRestriction **restrictions,
 int num_restrictions, int num_hash_buckets)
{
    GenerativeProofMachine *generativeProofMachine = gtd_malloc(sizeof(GenerativeProofMachine));
    generativeProofMachine->graph_buckets = gtd_malloc(sizeof(GraphNode *) * num_hash_buckets);
    for(int i = 0; i < num_hash_buckets; ++i)
    {
        generativeProofMachine->graph_buckets[i] = NULL;
    }
    generativeProofMachine->restrictions = restrictions;
    generativeProofMachine->num_restrictions = num_restrictions;
    generativeProofMachine->num_hash_buckets = num_hash_buckets;
    generativeProofMachine->num_graphs = 0;

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
    int hash = get_graph_hash(graph);
    int hash_bucket = hash % machine->num_hash_buckets;
    if(machine->graph_buckets[hash_bucket] == NULL)
    {
        machine->graph_buckets[hash_bucket] = gtd_malloc(sizeof(GraphNode));
        machine->graph_buckets[hash_bucket]->next = NULL;
    }
    else
    {
        GraphNode *oldList = machine->graph_buckets[hash_bucket];
        machine->graph_buckets[hash_bucket] = gtd_malloc(sizeof(GraphNode));
        machine->graph_buckets[hash_bucket]->next = oldList;
    }
    machine->graph_buckets[hash_bucket]->graph = graph;
    machine->num_graphs++;
}

void clear_graphs(GenerativeProofMachine *machine, int delete)
{
    if(delete)
    {
        for(int i = 0; i < machine->num_hash_buckets; ++i)
        {
            GraphNode *graphNode = machine->graph_buckets[i];
            while(graphNode != NULL)
            {
                GraphNode *nextNode = graphNode->next;
                destroy_graph(graphNode->graph);
                gtd_free(graphNode);
                graphNode = nextNode;
            }
            gtd_free(graphNode);
        }
    }
    machine->num_graphs = 0;
    machine->graph_buckets = gtd_malloc(sizeof(GraphNode *) * machine->num_hash_buckets);
    for(int i = 0; i < machine->num_hash_buckets; ++i)
    {
        machine->graph_buckets[i] = NULL;
    }
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

    int hash = get_graph_hash(graph);
    int hash_bucket = hash % machine->num_hash_buckets;
    GraphNode *graphNode = machine->graph_buckets[hash_bucket];
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

int generate_graphs_from_bucket(
    GenerativeProofMachine *machine,
    GraphNode *bucket)
    {
        int graphs_added = 0;
        GraphNode *node = bucket;
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
                    graphs_added++;
                }
            }

            node = node->next;
        }

        return graphs_added;
    }

int execute_generative_proof_machine(GenerativeProofMachine *machine)
{
    if(machine->num_restrictions == 0)
    {
        return 0;
    }

    GraphNode **oldGraphs = NULL;
    while(1)
    {
        printf("Machine num graphs: %d\n", machine->num_graphs);
        oldGraphs = machine->graph_buckets;
        clear_graphs(machine, 0);
        int graphs_added = 0;
        for(int bucket = 0; bucket < machine->num_hash_buckets; ++bucket)
        {
            // printf("%d\n", bucket);
            // generate_from_bucket
            int to_add = generate_graphs_from_bucket(machine, oldGraphs[bucket]);
            graphs_added += to_add;
        }

        if(graphs_added == 0)
        {
            break;
        }
    }

    /*
    */

   for(int bucket = 0; bucket < machine->num_hash_buckets; ++bucket)
   {
        GraphNode *node = oldGraphs[bucket];
        int count = 0;
        while(node != NULL)
        {
            node = node->next;
            count++;
        }
        printf("%3d: %3d\n", bucket, count);
   }

    FILE *out_file = fopen("matrices.txt", "w");

    for(int bucket = 0; bucket < machine->num_hash_buckets; ++bucket)
    {
        GraphNode *node = oldGraphs[bucket];
        while(node != NULL)
        {
            Graph *g = node->graph;
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
            node = node->next;
        }
    }

    fclose(out_file);

    return 1;
}