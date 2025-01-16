/**
 * \file proof_tree.c
 * \brief implementation of functions from proof_tree.h
 */
#define IS_GENERATIVE_MODULE_COMPONENT
#include <string.h>
#include "proof_tree.h"


ProofNode *create_proof_node(void)
{
    ProofNode *node = gtd_malloc(sizeof(ProofNode));
    node->message = NULL;
    node->subtree = NULL;
    node->next = NULL;
    return node;
}


ProofTree *create_proof_tree(void)
{
    ProofTree *tree = gtd_malloc(sizeof(ProofTree));
    tree->depth = 0;
    tree->head = NULL;
    tree->tail = NULL;
    return tree;
}

void delete_proof_tree(ProofTree *tree)
{
    if (tree == NULL)
    {
        return;
    }

    ProofNode *current = tree->head;
    ProofNode *next;

    while (current != NULL)
    {
        next = current->next;
        if (current->subtree != NULL)
        {
            delete_proof_tree(current->subtree);
        }
        free(current);
        current = next;
    }

    free(tree);
}


void append_proof_node(ProofTree *tree, ProofNode *node)
{
    if(tree->head == NULL)
    {
        tree->head = node;
        tree->tail = node;
    }
    else
    {
        tree->tail->next = node;
        tree->tail = node;
    }
}


void write_proof_node(ProofNode *node, FILE *outFile, int depth)
{
    if(node->message != NULL)
    {
        for(int i = 0; i < depth; ++i)
        {
            fprintf(outFile, "\t");
        }
        fprintf(outFile, "%s\n", node->message);
    }

    write_proof_tree(node->subtree, outFile, depth+1);
}


void write_proof_tree(ProofTree *tree, FILE *outFile, int depth)
{
    if(tree == NULL)
    {
        return;
    }

    ProofNode *proofNode = tree->head;
    while(proofNode != NULL)
    {
        write_proof_node(proofNode, outFile, depth);
        proofNode = proofNode->next;
    }
}