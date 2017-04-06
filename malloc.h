#include "param.h"

#ifndef MALLOC_H
#define MALLOC_H

/* Save metadata about each block of memory */
struct block_header{
	int isFree;
	size_t size;
	struct block_header *next;
};

struct block_header *get_free_block;
void *memalloc(size_t size, size_t current_task);
void free(void *block);

#endif  /* MALLOC_H */
