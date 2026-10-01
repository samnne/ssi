#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include "headers/node.h"
#include <stdlib.h>
#include <string.h>
#include "headers/emalloc.h"


typedef struct LinkedList LinkedList;
typedef struct Node Node;

// Functions for linked list operations
// insert, find, and remove nodes from the linked list
int insert(LinkedList *ll, int *key, char *value);
int find_node(LinkedList *ll, int *key);
int remove_node(LinkedList *ll, int *key);

struct LinkedList
{
    Node *head;
    int n;
    int (*insert)(struct LinkedList *ll, int *key, char *value);
    
    int (*find)(struct LinkedList *ll, int *key);
    int (*remove)(struct LinkedList *ll, int *key);
};
LinkedList *init_llist();

#endif

