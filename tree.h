#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

const int DEAD_SIZE = -1;
typedef char* value_t;
const value_t DEAD_VALUE = "YA V ZHOPE";

typedef enum Errors
{
    TREE_IS_OK = 0,
    TREE_IS_NULL = 1 << 0,
    TREE_LOGER_ERROR = 1 << 1,
    TREE_ROOT_IS_NULL = 1 << 2,
    NODE_PTR_IS_NULL = 1 << 3,
    TRY_REMOVE_ROOT = 1 << 4,
    TRY_REMOVE_NONLEAF = 1 << 5,
    ADD_NODE_ERROR = 1 << 6,
}Errors;

typedef struct Node
{
    value_t value;

    Node *left;
    Node *right;
}Node;

typedef struct Tree
{
    FILE* log;
    Node* root;
    int size;
    int errors;
}Tree;

typedef enum WhichChild
{
    Right = 1,
    Left = 0,
}WhichChild;

void TreeCtor( Tree* tree );
void TreeDtor( Tree* tree );
int NodeDtor( Node* node );
int InviteNode( Tree* tree, Node* node, WhichChild child, value_t value );
int RemoveNode( Tree* tree, Node* node );
int TreeDump( Tree* tree, FILE* logger );

#endif // #define TREE_H_