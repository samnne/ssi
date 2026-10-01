#ifndef _EMALLOC_H_
#define _EMALLOC_H_

#include <stdlib.h>
#include <stdio.h>

// Emalloc function to handle memory allocation errors and clean up code in ssi.c and other files
void *emalloc(size_t n);

#endif
