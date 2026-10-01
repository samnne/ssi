#ifndef _HISTORYFILE_H
#define _HISTORYFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/emalloc.h"
#include "headers/linkedlist.h"

void append_to_history_db(LinkedList *history, char *command);

void load_history_from_db(LinkedList *history);

#endif