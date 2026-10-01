

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include "headers/linkedlist.h"

// Function to get the current working directory
char cwd[1024];
char *get_cwd(char cwd[1024], size_t size)
{

    char *result = getcwd(cwd, size);
    if (result == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

    return result;
}

// Appends a command to the history file and the linked list
void append_to_history_db(LinkedList *history, char *command)
{


    FILE *file = fopen(cwd, "a");
    if (file == NULL)
    {
        perror("Error opening history file");
        return;
    }
    history->insert(history, &history->n, command);
    fprintf(file, "%s\n", command);
    fclose(file);
}


// Loads the history from the history file into the linked list
void load_history_from_db(LinkedList *history)
{

    get_cwd(cwd, sizeof(cwd));
    snprintf(cwd + strlen(cwd), sizeof("db/history.txt") + 1, "/db/history.txt");

    FILE *file = fopen(cwd, "r");
    if (file == NULL)
    {
        perror("Error opening history file");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        history->insert(history, &history->n, line);
    }

    fclose(file);
}