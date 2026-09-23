#include <headers/emalloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
  int key;
  char *value;
  Node *next;
};
Node *createNode(int key, char *command) {

  Node *newNode = (Node *)emalloc(sizeof(Node));
  newNode->key = key;
  newNode->value = strdup(command);
  if (newNode->value == NULL) {
    free(newNode);
    return NULL;
  }
  newNode->next = NULL;
  return newNode;
}
