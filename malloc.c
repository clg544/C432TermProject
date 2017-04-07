#include "malloc.h"

struct block_header *head, *tail;

/* The purpose of this struct is to save the metadata about each block of memory */
struct block_header{

	size_t size;
	unsigned is_free;
	struct block_header *next;

};

/* This function tries to find the memory address of the free space in heap 
 * which was caused by previous free() call. */
struct block_header *get_free_block(size_t size)
{
	struct block_header *current = head;
	/* kind of like a first-fit approach */
	while(current) {

		if (current->is_free && current->size >= size)
			return curr; /* can cause internal heap memory fragmentation */
		current = current->next;

	}
	return NULL;
}

/* malloc implementation */
void *memalloc(size_t size)
{
	size_t total_size;
	void *block;
	struct block_header *header;

	if (size < 1) 
		return -1;

	header = get_free_block(size);

	if (header) {

		header->is_free = 0;
		return (void*)(header + 1);

	}

	total_size = sizeof(struct block_header) + size;
	block = sbrk(total_size);

	if (block == (void*) -1) {

		return NULL;

	}

	header = block;
	header->size = size;
	header->is_free = 0;
	header->next = NULL;

	if (!head) head = header;

	if (tail) tail->next = header;

	tail = header;

	return (void*)(header + 1);
}

/* implementation of free() */

void free(void *block){

	struct block_header *header, *tmp;
	void *programbreak;

	if (!block)
		return;

	header = (struct block_header*)block - 1;

	programbreak = sbrk(0);
	if ((char*)block + header->size == programbreak) {

		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;

			while (tmp) {
				if(tmp->next == tail) {
					tmp->next = NULL;
					tail = tmp;
				}
				tmp = tmp->next;
			}
		}

		sbrk(0 - sizeof(struct block_header) - header->size);

		return;
	}

	header->is_free = 1;
}

/* simple implementation of Free */
void free(void *block){

	struct block_header *header, *tmp;
	void *programbreak;

	if (!block)
		return;
	
	header = (struct block_header*)block - 1;

	programbreak = sbrk(0);
	if ((char*)block + header->size == programbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (tmp) {
				if(tmp->next == tail) {
					tmp->next = NULL;
					tail = tmp;
				}
				tmp = tmp->next;
			}
		}
		sbrk(0 - sizeof(struct block_header) - header->size);
		return;
	}
	header->is_free = 1;
}
