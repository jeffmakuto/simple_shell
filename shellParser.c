#include "shell.h"

/**
 * cleanupAfterExecution - frees required memory
 *
 * @args: commands passed
 *
 * Return: void
 */
void cleanupAfterExecution(PROGARGS *args)
{
	if (args->tokens)
		freePtrs(args->tokens);
	if (args->buffer)
		free(args->buffer);
	if (args->cmd)
		free(args->cmd);

	args->buffer = NULL;
	args->cmd = NULL;
	args->tokens = NULL;
}

/**
 * freeArgs - frees all memory
 *
 * @args: commands passed
 *
 * Return: void
 */
void freeArgs(PROGARGS *args)
{
	if (args->fd != 0)
	{
		if (close(args->fd))
			perror(args->progName);
	}
	cleanupAfterExecution(args);
	freePtrs(args->envp);
}

/**
 * freePtrs - frees pointer memory
 *
 * @arr: array of pointers
 *
 * Return: void
 */
void freePtrs(char **arr)
{
	int i;

	if (arr != NULL)
	{
		for (i = 0; arr[i]; i++)
			free(arr[i]);

		free(arr);
		arr = NULL;
	}
}

/**
 * exitAction - exit the shell
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int exitAction(PROGARGS *args)
{
	int i;

	if (args->tokens[1] != NULL)
	{
		for (i = 0; args->tokens[1][i]; i++)
			if ((args->tokens[1][i] < '0' || args->tokens[1][i] > '9')
					&& args->tokens[1][i] != '+')
			{
				errno = 2;
				return (2);
			}
		errno = _atoi(args->tokens[1]);
	}
	freeArgs(args);
	exit(errno);
}
/**
* handleLogicalOperators - handle command execution with the && operator.
*
* @commands: array of the commands.
*
* @i: index
*
* @operators: array of the logical operators
*
* Return: index of the last command in the array
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

