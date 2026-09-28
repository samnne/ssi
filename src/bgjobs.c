#include "headers/emalloc.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef enum
{
  PROCESS_RUNNING,
  PROCESS_DONE,
  PROCESS_STOPPED
} ProcessState;

typedef struct bg_process
{
  int procces_id;
  pid_t pid;
  char *command;
  ProcessState state;
  struct bg_process *next;
} bg_process;

typedef struct
{
  bg_process *head;
  int next_procces_id;
} ProcessList;

// Initiliaze processlist
void init_process_list(ProcessList *processes)
{
  processes->head = NULL;
  processes->next_procces_id = 1;
}
// free process and free processlist here.
void free_process(bg_process *process)
{
  free(process->command);
  free(process);
}

void free_process_list(ProcessList *processes)
{
  bg_process *cur = processes->head;
  while (cur)
  {
    bg_process *next = cur->next;
    free_process(cur);
    cur = next;
  }
  processes->head = NULL;
}

/**
 * Add process to background process list
 * ARGS: ProcessList *processes, process list, pid_t pid: the pid, char* command: the command to add
 * RETURN: void
 */ 
void add_process(ProcessList *processes, pid_t pid, char *command)
{
  bg_process *process = emalloc(sizeof(bg_process));

  process->pid = pid;
  process->command = strdup(command);
  process->state = PROCESS_RUNNING;
  process->procces_id = processes->next_procces_id++;
  process->next = NULL;

  if (processes->head == NULL)
  {
    processes->head = process;
  }
  else
  {
    bg_process *cur = processes->head;
    while (cur->next)
      cur = cur->next;
    cur->next = process;
  }

  printf("%d: %d started\n\n", process->procces_id, pid);
}

/**
 * Removes a process from processlist, done once a process is finished
 * 
 * ARGS: ProcessList *processs: the global process list
 *        pid_t pid: a process id to search for and remove
 * RETURNS: void
 */
void remove_process(ProcessList *processes, pid_t pid)
{
  bg_process *cur = processes->head;
  bg_process *prev = NULL;

  while (cur)
  {
    if (cur->pid == pid)
    {
      if (prev)
        prev->next = cur->next;
      else
        processes->head = cur->next;
      
      free_process(cur);
      return;
    }
    prev = cur;
    cur = cur->next;
  }
}


/**
 * Updates the processes to check whether a bg process has finished
 * ARGS: the process list
 * RETURN: void
 */
void update_processes(ProcessList *processes)
{
  int status;
  pid_t pid;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    bg_process *cur = processes->head;
    while (cur)
    {
      if (cur->pid == pid)
      {
        cur->state = PROCESS_DONE;
        break;
      }

      cur = cur->next;
    }
  }
}

