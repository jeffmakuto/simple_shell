#include "shell.h"

/**
 * _realloc - Reallocates a memory block.
 * @ptr: Pointer to the existing memory block.
 * @new_size: The new size to allocate.
 *
 * Return: On success, returns a pointer to the newly reallocated memory block.
 * On failure, returns NULL.
 */
void *_realloc(void *ptr, size_t new_size)
{
	void *new_ptr;
	size_t min_size, i;

	/* If ptr is NULL, the behavior of _realloc is same as malloc(new_size)*/
	if (!ptr)
		return (malloc(new_size));

	/* If new_size is 0, the behavior of _realloc is same as free(ptr) */
	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}

	/* Allocate a new memory block with the new size */
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		/* If malloc fails, return NULL indicating failure to reallocate */
		return (NULL);
	}

	/* Determine the minimum size to copy from the old block to the new block */
	/* It should be the smaller of the old and new sizes */
	min_size = new_size;
	if (min_size > sizeof(ptr))
		min_size = sizeof(ptr);

	/* Copy the data from the old block to the new block */
	for (i = 0; i < min_size; i++)
		*((char *)new_ptr + i) = *((char *)ptr + i);

	/* Free the old block */
	free(ptr);
	return (new_ptr);
}