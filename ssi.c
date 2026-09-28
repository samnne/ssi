#include "headers/emalloc.h"
#include "headers/linkedlist.h"
#include "headers/node.h"
#include "headers/bgjobs.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define COMMAND_MAX 100
#define BLU "\x1B[1;34m"
#define RESET "\x1B[0m"

int ctrl_c_flag = 0;
int sigchild_flag = 0;

/**
 * Signal handler for terminal signals in the SSI
 * Args:
 *  sig: int, the signial
 */
void signal_handler(int sig)
{

  switch (sig)
  {
  case SIGINT:
    ctrl_c_flag = 1;
    return;
  case SIGCHLD:
    sigchild_flag = 1;
    return;
  }
}
// Handles command line signals
struct sigaction sa;
void initialize_sigaction()
{

  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  sa.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &sa, NULL);
}

/**
 * Function change_directories:
 *  The function adds the change directories functionality.
 * Args: ...
 * returns nothing
 */
int change_directories(char *prev_path, Node *dir)
{

  char final_path[COMMAND_MAX];
  if (dir == NULL)
  {
    snprintf(final_path, sizeof(final_path), "/home/%s", getlogin());
    chdir(final_path);
    return -1;
  }
  else if (dir->value[0] == '/')
  {
    snprintf(final_path, sizeof(final_path), "%s", dir->value);
    chdir(final_path);
    return -1;
  }
  snprintf(final_path, sizeof(final_path), "%s/%s", prev_path, dir->value);
  int child_process_id = chdir(final_path);

  return child_process_id;
}

/*
 *  Converts the linkedlist command into a stirng array
 *  Args: LinkedList *commands the pointer to the linked list of commands
 *    int count: the length of the linked list
 * */
char **command_to_string_array(LinkedList *commands, int count)
{

  char **argv = emalloc((count + 1) * sizeof(char *));

  int background = strncmp(commands->head->value, "bg",
                           sizeof(commands->head->value) - 1) == 0;
  Node *cur = background ? commands->head->next : commands->head;
  int i = 0;
  while (i < count && cur != NULL)
  {
    argv[i++] = cur->value;

    cur = cur->next;
  }
  argv[i] = NULL;
  return argv;
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

/**
 * Build bg_process command
 */
char *build_process_command(LinkedList *commands)
{
  int str_length = 0;
  Node *cur = commands->head->next;
  while (cur != NULL)
  {
    str_length += strlen(cur->value);
    cur = cur->next;
  }
  char *full_command = emalloc(str_length);
  cur = commands->head->next;
  strcpy(full_command, cur->value);
  cur = cur->next;
  while (cur != NULL)
  {
    strcat(full_command, " ");
    strcat(full_command, cur->value);
    cur = cur->next;
  }
  return full_command;
}

void build_executable_string(ProcessList *processes, bg_process *cur)
{
  // get command base from command string
  char *cpy = emalloc(strlen(cur->command));
  strcpy(cpy, cur->command);
  char *c_base = strtok(cpy, " ");

  // get path varaible to build where the program ran from.
  char *path = getenv("PATH");
  if (!path)
  {

    free(cpy);
    return;
  }
  char *path_cpy = emalloc(strlen(path) + 1);
  strcpy(path_cpy, path);
  char *token = strtok(path_cpy, ":");
  char test_path[PATH_MAX];
  char full_path[PATH_MAX];
  while (token != NULL)
  {
    snprintf(test_path, sizeof(full_path), "%s/%s", token, c_base);
    if (access(test_path, X_OK) == 0)
    {
      snprintf(full_path, sizeof(full_path), "%s/%s", token, cur->command);

      break;
    }
    token = strtok(NULL, ":");
  }

  // handle change in state and removing process from linked list
  if (cur->state == PROCESS_DONE)
  {

    printf("%d: %s %s\n", cur->pid, full_path, "has terminated");
    remove_process(processes, cur->pid);
  }
  else
  {
    printf("%d: %s\n", cur->pid, full_path);
  }
  free(cpy);
}

/*
Function: execute_command
forks and executes the command on the main machine.
Args: char* prompt, free the prompt after use to avoid memory leaks.
Returns: Nothing
*/
void execute_command(char *prompt, LinkedList *commands, ProcessList *processes, LinkedList *history)
{

  int print_history = strncmp(commands->head->value, "history", sizeof("history")) == 0;
  if (print_history)
  {

    Node *cur = history->head;
    while (cur)
    {
      printf("%s\n", cur->value);
      cur = cur->next;
    }
    return;
  }

  int bglist_bool = strncmp(commands->head->value, "bglist", sizeof(commands->head->value) - 1) == 0;
  if (bglist_bool)
  {
    bg_process *cur = processes->head;
    int count = 0;
    while (cur != NULL)
    {
      build_executable_string(processes, cur);
      count++;
      cur = cur->next;
    }

    printf("Total Background Jobs: %d\n", count);
    free(prompt);
    return;
  }
  int change_dir = strncmp(commands->head->value, "cd", sizeof(commands->head->value) - 1) ==
                   0;
  if (change_dir)
  {
    char prev_path[PATH_MAX + 1];
    prev_path[PATH_MAX] = '\0';
    getcwd(prev_path, sizeof(prev_path));
    change_directories(prev_path, commands->head->next);
    free(prompt);
    return;
  }

  int background = strncmp(commands->head->value, "bg",
                           sizeof(commands->head->value) - 1) == 0;
  if (background && commands->head->next == NULL)
  {
    printf("bg: missing command\n");

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
    if (background)
    {

      int dev_null = open("/dev/null", O_WRONLY);

      dup2(dev_null, STDOUT_FILENO);
      dup2(dev_null, STDERR_FILENO);

      close(dev_null);
    }
    signal(SIGINT, SIG_DFL);

    char **argv = command_to_string_array(commands, commands->n);
    execvp(argv[0], argv);
    exit(1);
  }
  else
  {
    int status;
    if (background)
    {
      char *full_command = build_process_command(commands);

      add_process(processes, pid, full_command);
      free(prompt);
      return;
    }
    waitpid(pid, &status, 0);
  }
  free(prompt);
}

char* get_branch_name()
{
  int status = system("git rev-parse --is-inside-work-tree > /dev/null 2>&1");
  if (status != 0)
  {
    return NULL;
  }
  char *branch = emalloc(sizeof(char) * 256);
  FILE *fp = popen("git branch --show-current", "r");
  if (fp == NULL)
  {
    return NULL;
  }
  if (

      fgets(branch, sizeof(branch), fp))
  {
    branch[strcspn(branch, "\n")] = 0;
  }
  if (branch > 0)
  {
    return branch;
  }
  return NULL;
}

void printprompt()
{
  char *branch_name = get_branch_name();

  char *username = getlogin();
  if (username == NULL)
  {
    printf("Couldn't retrive username\n");
    return;
  }
  char hostname[HOST_NAME_MAX + 1];
  hostname[HOST_NAME_MAX] = '\0';
  if (gethostname(hostname, sizeof(hostname) - 1) == 0)
  {
  }
  else
  {
    printf("Couldn't retrive hostname\n");
    return;
  }
  char cwd[PATH_MAX + 1];
  cwd[PATH_MAX] = '\0';
  getcwd(cwd, sizeof(cwd));
  printf(BLU "%s@%s: %s * %s > " RESET, username, hostname, cwd, branch_name);
}

void add_to_history(LinkedList *history, char *prompt, int id)
{

  history->insert(history, &id, prompt);
}

int main()
{
  // init the sigaction to handle the actions
  initialize_sigaction();

  // initialize background process
  ProcessList processes;
  init_process_list(&processes);
  LinkedList *history = init_llist();

  // The main event loop
  while (1)
  {

    if (sigchild_flag)
    {
      sigchild_flag = 0;
      update_processes(&processes);
    }

    printprompt();
    // the prompt to read commands
    char *prompt = NULL;
    size_t size = 0;
    ssize_t characters_read = getline(&prompt, &size, stdin);
    if (characters_read == -1)
    {

      // handles the ^C flag in terminal
      if (ctrl_c_flag)
      {
        ctrl_c_flag = 0;
        clearerr(stdin);
        free(prompt);

        printf("\n\n");
        continue;
      }
      free(prompt);
      exit(0);
    }

    /**
     * Exits the program
     * free's prompt and free process_list
     */
    if (strncmp("exit\n", prompt, strlen("exit\n")) == 0)
    {
      free_process_list(&processes);
      Node *cur = history->head;
      while (cur)
      {

        Node *next = cur->next;
        free(cur);
        cur = next;
      }
      free(history);
      free(prompt);
      exit(0);
    }
    prompt[strcspn(prompt, "\n")] = '\0';
    add_to_history(history, prompt, history->n);
    LinkedList *commands = store_command(prompt);
    if (commands->head == NULL)
    {
      free(commands->head);
      free(commands);

      printf("\n");
      free(prompt);
      continue;
    }

    execute_command(prompt, commands, &processes, history);
  }

  return 0;
}
