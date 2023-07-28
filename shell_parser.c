#include "shell.h"

/**
 * cleanup_after_execution - frees required memory
 *
 * @args: commands passed
 *
 * Return: void
 */
void cleanup_after_execution(prog_args *args)
{
	if (args->tokens)
		free_ptrs(args->tokens);
	if (args->buffer)
		free(args->buffer);
	if (args->cmd)
		free(args->cmd);

	args->buffer = NULL;
	args->cmd = NULL;
	args->tokens = NULL;
}

/**
 * free_args - frees all memory
 *
 * @args: commands passed
 *
 * Return: void
 */
void free_args(prog_args *args)
{
	if (args->fd != 0)
	{
		if (close(args->fd))
			perror(args->prog_name);
	}
	cleanup_after_execution(args);
	free_ptrs(args->envp);
}

/**
 * free_ptrs - frees pointer memory
 *
 * @arr: array of pointers
 *
 * Return: void
 */
void free_ptrs(char **arr)
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
 * exit_action - exit the shell
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int exit_action(prog_args *args)
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
	free_args(args);
	exit(errno);
}
/**
* handle_logical_operators - handle command execution with the && operator.
*
* @commands: array of the commands.
*
* @i: index
*
* @operators: array of the logical operators
*
* Return: index of the last command in the array
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

