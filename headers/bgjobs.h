#ifndef _BGJOBS_H
#define _BGJOBS_H

#include "headers/emalloc.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Process State enum to track the state of background processes
typedef enum
{
    PROCESS_RUNNING,
    PROCESS_DONE,
    PROCESS_STOPPED
} ProcessState;


// Struct to represent a background process
typedef struct bg_process
{
    int process_id;
    pid_t pid;
    char *command;
    ProcessState state;
    struct bg_process *next;
} bg_process;


// Struct to represent a list of background processes
typedef struct
{
    bg_process *head;
    int next_process_id;
} ProcessList;


/**
 * Run this function to initialize the process list.
 * ARGS: ProcessList *processes: the global process list
 * RETURN: void
 */
void init_process_list(ProcessList *processes);

/**
 * Frees a process and its associated resources.
 * ARGS: bg_process *process: the process to free
 * RETURN: void
 */
void free_process(bg_process *process);
/**
 * Frees the entire process list and all associated resources.
 * ARGS: ProcessList *processes: the global process list
 * RETURN: void
 */
void free_process_list(ProcessList *processes);


/**
 * Process Operations, Add, Remove, Update
 * ARGS: ProcessList *processes: the global process list
 *       pid_t pid: the process id to add/remove/update
 *       char *command: the command associated with the process (for add)
 * RETURN: void
 */
void add_process(ProcessList *processes, pid_t pid, const char *command);
void remove_process(ProcessList *processes, pid_t pid);
void update_processes(ProcessList *processes);


#endif