#include "shell.h"

/**
 * _strspn - gets the lenth of a prefix substring
 *
 * @s: string to search
 *
 * @accept: bytes to accept
 *
 * Return: the no. of bytes in initial segment of @s
 */
unsigned int _strspn(char *s, const char *accept)
{
	unsigned int i, j;

	for (i = 0; *(s + i) != '\0'; i++)
	{
		for (j = 0; *(accept + j) != '\0'; j++)
		{
			if (*(s + i) == *(accept + j))
			{
				break;
			}

		}
		if (*(accept + j) == '\0')
		{
			break;
		}

	}
	return (i);
}

/**
 * _getline - Read a line from a file stream and dynamically allocate
 * memory for it.
 *
 * This function reads a line from the specified file stream and dynamically
 * allocates memory to store the line contents. The function will automatically
 * resize the buffer as needed to accommodate the entire line.
 *
 * @lineptr: Pointer to the buffer that will hold the line contents.
 * @n: Pointer to the initial size of the buffer. Will be updated if the
 * buffer is resized.
 * @stream: The file stream to read the line from.
 *
 * Return: The number of characters read, excluding the null-terminator,
 * or -1 on failure.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	size_t buffer_size, pos;
	int c;
	char *new_buffer;

	if (lineptr == NULL || n == NULL || stream == NULL)

		return (-1); /* Invalid arguments */
	/* Set initial buffer size and allocate memory if needed */
	buffer_size = *n;
	if (*lineptr == NULL)
	{
		*lineptr = (char *)malloc(buffer_size);
		if (*lineptr == NULL)

			return (-1); /* Memory allocation failed */
	}
	pos = 0;
	while ((c = fgetc(stream)) != EOF)
	{
		/* Resize the buffer if needed */
		if (pos >= buffer_size - 1)
		{
			buffer_size *= 2; /* Double the buffer size */
			new_buffer = (char *)realloc(*lineptr, buffer_size);
			if (new_buffer == NULL)
				return (-1); /* Memory allocation failed */
			*lineptr = new_buffer;
			*n = buffer_size;
		}
		/* Store the character in the buffer */
		(*lineptr)[pos++] = (char)c;

		/* Break loop if we encounter a newline character */
		if (c == '\n')
			break;
	}
	/* Null-terminate the line */
	(*lineptr)[pos] = '\0';
	return (pos);
}
