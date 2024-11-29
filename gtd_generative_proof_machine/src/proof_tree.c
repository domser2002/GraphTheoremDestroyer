#include <string.h>
#include "proof_tree.h"
#include "common.h"


ProofNode *initProofNode(void)
{
    ProofNode *node = gtd_malloc(sizeof(ProofNode));
    node->message = NULL;
    node->subtree = NULL;
    node->next = NULL;
    return node;
}


ProofTree *initProofTree(void)
{
    ProofTree *tree = gtd_malloc(sizeof(ProofTree));
    tree->depth = 0;
    tree->nodes = NULL;
    tree->nodesTail = NULL;
    return tree;
}


void addProofNode(ProofTree *tree, ProofNode *node)
{
    if(tree->nodes == NULL)
    {
        tree->nodes = node;
        tree->nodesTail = node;
    }
    else
    {
        tree->nodesTail->next = node;
        tree->nodesTail = node;
    }
}


void writeNode(ProofNode *node, FILE *outFile, int depth)
{
    if(node->message != NULL)
    {
        for(int i = 0; i < depth; ++i)
        {
            fprintf(outFile, " ");
        }
        fprintf(outFile, "%s\n", node->message);
    }

    writeProof(node->subtree, outFile);
}


void writeProof(ProofTree *tree, FILE *outFile)
{
    if(tree == NULL)
    {
        return;
    }

    int depth = tree->depth;
    ProofNode *proofNode = tree->nodes;
    while(proofNode != NULL)
    {
        writeNode(proofNode, outFile, depth);
        proofNode = proofNode->next;
    }
}