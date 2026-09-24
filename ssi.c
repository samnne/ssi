#include <limits.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "headers/emalloc.h"
#include "headers/node.h"
#include "headers/linkedlist.h"

#define COMMAND_MAX 100

struct bg_process
{
  pid_t pid;
  char *command;
  struct bg_process *next;
};

int ctrl_c_flag = 0;

void signal_handler(int sig)
{

  switch (sig)
  {
  case SIGINT:
    ctrl_c_flag = 1;
    return;

  }
}

struct sigaction
    sa;
void initialize_sigaction()
{
  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
}

/**
 * Function change_directories:
 *  The function adds the change directories functionality.
 * Args: ...
 * returns nothing
 */
int change_directories(char *prev_path, Node* dir)
{
  
  char final_path[COMMAND_MAX];
  if (dir == NULL) {
    snprintf(final_path, sizeof(final_path),"/home/%s", getlogin());
    chdir(final_path);
    return -1;
  } else if (dir->value[0] == '/'){
    snprintf(final_path, sizeof(final_path),"%s", dir->value);
    chdir(final_path);
    return -1;
  }
  snprintf(final_path, sizeof(final_path), "%s/%s", prev_path, dir->value);
  int child_process_id = chdir(final_path);

  return child_process_id;
}



/**
 * Function store_command
 * Args: the prompt the user inputted
 * Returns: LinkedList* or command linked list
 */
LinkedList *store_command(char *prompt)
{
  LinkedList *ll = init_llist();
  char *deli = " \t\r\n";
  char *token = strtok(prompt, deli);
  int i = 0;
  while (token != NULL && i < COMMAND_MAX - 1)
  {
    ll->insert(ll, &i, token);
    i++;
    token = strtok(NULL, deli);
  }

  return ll;
}
/*
Function: execute_command
forks and executes the command on the main machine.
Args: char* prompt, free the prompt after use to avoid memory leaks.
Returns: Nothing
*/
void execute_command(char *prompt, LinkedList *commands)
{
  
  if (strncmp(commands->head->value, "cd", sizeof(commands->head->value) - 1) == 0)
  {
    char cwd[PATH_MAX + 1];
    cwd[PATH_MAX] = '\0';
    getcwd(cwd, sizeof(cwd));
    
    change_directories(cwd, commands->head->next);
    return;
  }

  pid_t pid = fork();
  if (pid < 0)
  {
    free(commands->head);
    free(commands);
    free(prompt);
    exit(1);
  }
  else if (pid == 0)
  {
    signal(SIGINT, SIG_DFL);

    int count = commands->n;
    char **argv = emalloc(sizeof(count + 1) * sizeof(char *));
    Node *cur = commands->head;
    for (int i = 0; i < count;i++)
    {
      argv[i] = cur->value;
     
      cur = cur->next;
    }
    argv[count] = NULL;
    execvp(argv[0], argv);
    exit(1);
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
  }
  free(prompt);
}

void printprompt()
{
  char *username = getlogin();
  if (username == NULL)
  {
    printf(" fail");
    return;
  }
  char hostname[HOST_NAME_MAX + 1];
  hostname[HOST_NAME_MAX] = '\0';
  if (gethostname(hostname, sizeof(hostname) - 1) == 0)
  {
  }
  else
  {
    printf("fail");
    return;
  }
  char cwd[PATH_MAX + 1];
  cwd[PATH_MAX] = '\0';
  getcwd(cwd, sizeof(cwd));
  printf("%s@%s: %s > ", username, hostname, cwd);
}

int main()
{
  // init the sigaction to handle the ctrl c flag
  initialize_sigaction();
  while (1)
  {
    sigaction(SIGINT, &sa, NULL);

    printprompt();
    char *prompt = NULL;
    size_t size = 0;
    ssize_t characters_read = getline(&prompt, &size, stdin);
    if (characters_read == -1)
    {
      if (ctrl_c_flag)
      {
        ctrl_c_flag = 0;
        clearerr(stdin);
        free(prompt);
        printf("\n");
        continue;
      }
      free(prompt);
      exit(0);
    }
    prompt[strcspn(prompt, "\n")] = '\0';
    if (strncmp("exit", prompt, strlen(prompt) - 1) == 0)
    {

      free(prompt);
      exit(0);
    }

    LinkedList *commands = store_command(prompt);
    
    if (commands->head == NULL)
    {
      free(commands->head);
      free(commands);

      free(prompt);
      continue;
    }
    execute_command(prompt, commands);
  }

  return 0;
}
