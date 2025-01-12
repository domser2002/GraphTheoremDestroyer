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

/**
 * \brief Function to create an empty ProofNode
 * \return Pointer to the newly created ProofNode
 */
ProofNode *create_proof_node(void);

/**
 * \brief Function to create an empty ProofTree
 * \return Pointer to the newly created ProofTree
 */
ProofTree *create_proof_tree(void);

/**
 * \brief Function to free memory of a ProofTree and all of its children
 * \param tree - pointer to the ProofTree structure which memory should be freed
 */
void delete_proof_tree(ProofTree *tree);

/**
 * \brief Function to append new ProofNode to the ProofTree
 * \param tree - pointer to the ProofTree structure that node will be appended into
 * \param node - pointer to the ProofNode structure that will be appended to tree
 */
void append_proof_node(ProofTree *tree, ProofNode *node);

/**
 * \brief Function to write proof from ProofNode to the output stream
 * \param node - pointer to the ProofNode structure from which proof should be extracted
 * \param outFile - pointer to the FILE, an output stream to which proof will be written
 * \param depth - int, an identation level
 */
void write_proof_node(ProofNode *node, FILE *outFile, int depth);

/**
 * \brief Function to write proof from ProofTree to the output stream
 * \param tree - pointer to the ProofTree structure from which proof should be extracted
 * \param outFile - pointer to the FILE, an output stream to which proof will be written
 */
void write_proof_tree(ProofTree *tree, FILE *outFile);
#endif
#endif