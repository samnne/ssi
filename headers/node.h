typedef struct Node Node;

struct Node {
  int key;
  char *value;
  Node *next;
};

Node *createNode(int key, char *command);
