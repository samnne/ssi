#include <stdlib.h>
#include <stdio.h>
#include "headers/emalloc.h"

// Error malloc, to handle memory errors and clean up code 
// in ssi.c and other files
void *emalloc(size_t n) {
    void *p; 

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n); 
        exit(1);
    }   

    return p;
}
