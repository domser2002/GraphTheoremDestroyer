#include <string.h>
#include "common.h"

typedef struct ProofNode ProofNode;

typedef struct ProofTree
{
    int depth;
    ProofNode *nodes;
    ProofNode *nodesTail;
}ProofTree;

typedef struct ProofNode
{
    const char *message;
    ProofTree *subtree;
    ProofNode *next;
}ProofNode;

// ===== Methods =====

ProofNode *initProofNode(void);

ProofTree *initProofTree(void);

void addProofNode(ProofTree *tree, ProofNode *node);

void writeNode(ProofNode *node, FILE *outFile, int depth);

void writeProof(ProofTree *tree, FILE *outFile);