#include "shell.h"

/**
* _getline - read input from the  prompt.
* @info: commands passed
* Return: read bytes.
*/
int _getline(info_t *info)
{
	char buffer[BUFFER_SIZE] = {'\0'};
	static char *commands[INITIAL_ENV_SIZE] = {NULL};
	static char operators[INITIAL_ENV_SIZE] = {'\0'};
	ssize_t bytes_read, i = 0;

	/* check if there are no more commands in the array */
	/* and checks the logical operators */
	if (!commands[0] || (operators[0] == '&' && errno != 0) ||
		(operators[0] == '|' && errno == 0))
	{
		/*free the memory allocated in the array if it exists */
		for (i = 0; commands[i]; i++)
		{
			free(commands[i]);
			commands[i] = NULL;
		}

		/* read from the file descriptor into buffer */
		bytes_read = read(info->fd, &buffer, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* check for \n or ; */
		i = 0;
		do {
			commands[i] = _strdup(_strtok(i ? NULL : buffer, "\n;"));
			/*checks and split for && and || operators*/
			i = handle_logical_operators(commands, i, operators);
		} while (commands[i++]);
	}

	/*obtain the next command (command 0) and remove it for the array*/
	info->buff = commands[0];
	for (i = 0; commands[i]; i++)
	{
		commands[i] = commands[i + 1];
		operators[i] = operators[i + 1];
	}

	return (_strlen(info->buff));
}


/**
* handle_logical_operators - handles && and || operators
* @commands: array of the commands.
* @i: index in the array_commands to be checked
* @operators: array of the logical operators for each previous command
*
* Return: index of the last command in the array_commands.
*/
int handle_logical_operators(char *commands[], int i, char operators[])
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
