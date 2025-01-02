#ifndef PROOF_TREE_H
#define PROOF_TREE_H
#ifdef IS_GENERATIVE_MODULE_COMPONENT
#include <string.h>
#include "common.h"
typedef struct ProofNode ProofNode;

typedef struct ProofTree
{
    int depth;
    ProofNode *head;
    ProofNode *tail;
} ProofTree;

typedef struct ProofNode
{
    const char *message;
    ProofTree *subtree;
    ProofNode *next;
} ProofNode;

ProofNode *create_proof_node(void);
ProofTree *create_proof_tree(void);
void append_proof_node(ProofTree *tree, ProofNode *node);
void write_proof_node(ProofNode *node, FILE *outFile, int depth);
void write_proof_tree(ProofTree *tree, FILE *outFile);
#endif
#endif