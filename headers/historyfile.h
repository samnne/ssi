#ifndef _HISTORYFILE_H
#define _HISTORYFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/emalloc.h"
#include "headers/linkedlist.h"

// Function to get the current working directory
char *get_cwd(char cwd[1024], size_t size);

// Appends a command to the history file and the linked list
void append_to_history_db(LinkedList *history, char *command);

// Loads the history from the history file into the linked list
void load_history_from_db(LinkedList *history);

#endif