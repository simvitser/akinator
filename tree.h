#ifndef _TREE_H
#define _TREE_H

#include <stdbool.h>
#include <stdlib.h>

typedef char* treeElement_t;
#define TREE_ELEMENT_FORMATER "%s"

#define MAX_NODE_LEN 100

struct _node_t {
    struct _node_t *left;
    struct _node_t *right;

    treeElement_t data;
};

typedef struct _node_t node_t;

node_t* treeAddNode(node_t *root, treeElement_t el);

void   inPrint(const node_t *node);
void  prePrint(const node_t *node);
void postPrint(const node_t *node);

void treeDestroy(node_t *node);

bool treeWrite(node_t *node, const char *filename);

node_t* treeRead(const char *filename);

void treeDump(node_t *root);


#endif
