#include "shell.h"

/**
 * _realloc - Reallocates a memory block.
 *
 * @ptr: Pointer to the existing memory block.
 *
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

/**
 * _atoi - Convert a string to an integer.
 *
 * @s: The pointer to convert
 *
 * Return: A integer
 */
int _atoi(char *s)
{
	int c = 0;
	unsigned int ni = 0;
	int min = 1;
	int isi = 0;

	while (s[c])
	{
		if (s[c] == 45)
		{
			min *= -1;
		}

		while (s[c] >= 48 && s[c] <= 57)
		{
			isi = 1;
			ni = (ni * 10) + (s[c] - '0');
			c++;
		}

		if (isi == 1)
		{
			break;
		}

		c++;
	}

	ni *= min;
	return (ni);
}

/**
* _getline - read one line from the prompt.
*
* @args: Arguments passed
*
* Return: read bytes.
*/
int _getline(PROGARGS *args)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *commands[INITIAL_ENVP_SIZE] = {NULL};
	static char operators[INITIAL_ENVP_SIZE] = {NULL};
	ssize_t bytesRead, i = 0;
	bool hasError = false;

	/* check if there are no  more commands in the array */
	/* and checks the logical operators */
	if (!commands[0] || (operators[0] == '&' && hasError) ||
		(array_operators[0] == '|' && !hasError))
	{
		/*free the memory allocated in the array if it exists */
		for (i = 0; commands[i]; i++)
		{
			free(commands[i]);
			commands[i] = NULL;
		}

		/* read from the file descriptor int to buff */
		bytesRead = read(args->fd, &buff, BUFFER_SIZE - 1);
		if (bytesRead == 0)
			return (-1);

		/* split lines for \n or ; */
		i = 0;
		do {
			commands[i] = _strdup(_strtok(i ? NULL : buff, "\n;"));
			/*checks and split for && and || operators*/
			i = handleLogicalOperators(commands, i, operators);
		} while (commands[i++]);
	}

	/*obtains the next command (command 0) and remove it from the array*/
	args->buffer = commands[0];
	for (i = 0; commands[i]; i++)
	{
		commands[i] = commands[i + 1];
		operators[i] = operators[i + 1];
	}

	return (_strlen(args->buffer));
}


/**
* handleLogicalOperators - Handle command execution with the && and ||
*
* @commands: array of the commands.
*
* @i: index
*
* @operators: array of the logical operators
*
* Return: index of the last command in the commands array
*/
int handleLogicalOperators(char *commands[], int i, char operators[])
{
	char *temp = NULL;
	int j;

	/* checks for the & char in the command line*/
	for (j = 0; commands[i] != NULL  && commands[i][j]; j++)
	{
		if (commands[i][j] == '&' && commands[i][j + 1] == '&')
		{
			/* split the line when chars && was found */
			temp = commands[i];
			commands[i][j] = '\0';
			commands[i] = _strdup(commands[i]);
			commands[i + 1] = _strdup(temp + j + 2);
			i++;
			operators[i] = '&';
			free(temp);
			j = 0;
		}
		if (commands[i][j] == '|' && commands[i][j + 1] == '|')
		{
			/* split the line when chars || was found */
			temp = commands[i];
			commands[i][j] = '\0';
			commands[i] = _strdup(commands[i]);
			commands[i + 1] = _strdup(temp + j + 2);
			i++;
			operators[i] = '|';
			free(temp);
			j = 0;
		}
	}
	return (i);
}

/**
 * buffcat - append string at end of buffer
 *
 * @buffer: buffer to be filled
 *
 * @addStr: string to be put in buffer
 *
 * Return: length of concatenated buffer
 */
int buffcat(char *buffer, char *addStr)
{
	int len, i;

	len = _strlen(buffer);
	for (i = 0; addStr[i]; i++)
	{
		buffer[len + i] = addStr[i];
	}
	buffer[len + i] = '\0';
	return (len + i);
}
