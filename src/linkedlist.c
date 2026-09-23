#include <headers/node.h>
#include <stdlib.h>
#include <string.h>
#include <headers/emalloc.h>

#define NODEEMPTY NULL

/**
 * LinkedList Struct to handle find, remove, and insert command values
 *
 * Think of the head as 'ls' and then every value after that as a seperate node.
 */
typedef struct LinkedList
{
    Node *head;
    int n;
    int (*insert)(struct LinkedList *ll, int *key, char *value);

    int (*find)(struct LinkedList *ll, int *key);
    int (*remove)(struct LinkedList *ll, int *key);
} LinkedList;

/**
 * int insert
 * args: the linked list, the key, and value of the node you want to create
 * return 0 on success, exits the program on failure.
 *
 */
int insert(LinkedList *ll, int *key, char *value)
{
    Node *node = createNode(*key, value);
    if (ll->head == NODEEMPTY)
    {

        ll->head = node;
        ll->n += 1;
        return 0;
    }

    Node *cur = ll->head;
    while (cur->next != NODEEMPTY)
    {
        cur = cur->next;
    }

    (cur)->next = node;
    ll->n += 1;
    return 0;
}

/**
 * Finds the node given the key, in this case int key can also be pid_t
 */
int find_node(LinkedList *ll, int *key)
{
    if (ll->head == NODEEMPTY)
    {
        return -1;
    }
    Node *cur = ll->head;
    while (cur != NODEEMPTY)
    {
        if (cur->key == *key)
        {
            return cur->key;
        }
        cur = cur->next;
    }

    return -1;
}

// Removes a node given by the key
// Usage: ll->remove_node
int remove_node(LinkedList *ll, int *key)
{
    if (ll->head == NODEEMPTY)
    {
        return 0;
    }
    Node *cur = ll->head;
    Node *prev = NODEEMPTY;

    while (cur != NODEEMPTY)
    {
        if (cur->key == *key)
        {
            if (prev == NODEEMPTY)
            {
                ll->head = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
            ll->n -= 1;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/**
 * Initialize the linked list and returns the newly created linked list
 * The constructor.
 */
LinkedList *init_llist()
{
    LinkedList *ll = emalloc(sizeof(LinkedList));

    ll->n = 0;
    ll->insert = insert;
    ll->find = find_node;
    ll->remove = remove_node;
    ll->head = NODEEMPTY;

    return ll;
}
