#ifndef NODE_H
#define NODE_H
typedef struct Node Node;

struct Node {
  int key;
  char *value;
  Node *next;
};
// create node function for linkedlist.c, only use this if you need to create a new / dummy Node.
Node *createNode(int key, char *command);

#endif 
