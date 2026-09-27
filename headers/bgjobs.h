#ifndef _BGJOBS_H
#define _BGJOBS_H

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
    int process_id;
    pid_t pid;
    char *command;
    ProcessState state;
    struct bg_process *next;
} bg_process;

typedef struct
{
    bg_process *head;
    int next_process_id;
} ProcessList;

void init_process_list(ProcessList *processes);
void free_process(bg_process *process);
void free_process_list(ProcessList *processes);
void add_process(ProcessList *processes, pid_t pid, const char *cmdline);
void remove_process(ProcessList *processes, pid_t pid);
void update_processes(ProcessList *processes);
#endif