#include "shell.h"

/**
 * replaceVariables - Expands the variable
 *
 * @args: Arguments passed
 *
 * Return: Void
 */
void replaceVariables(PROGARGS *args)
{
	int i, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (args->buffer == NULL)
		return;
	buffcat(line, args->buffer);
	for (i = 0; line[i]; i++)
		if (line[i] == '#')
			line[i--] = '\0';
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			longToStr(errno, expansion, 10);
			buffcat(line, expansion);
			buffcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			longToStr(getpid(), expansion, 10);
			buffcat(line, expansion);
			buffcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		else if (line[i] == '$')
		{
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			temp = _getenv(expansion, args);
			line[i] = '\0', expansion[0] = '\0';
			buffcat(expansion, line + i + j);
			temp ? buffcat(line, temp) : 1;
			buffcat(line, expansion);
		}
	if (!_strncmp(args->buffer, line, 0))
	{
		free(args->buffer);
		args->buffer = _strdup(line);
	}
}

/**
 * _getenv - get value of environment variable
 *
 * @name: environment variable
 *
 * @args: Arguments passed
 *
 * Return: ptr to value of var or NULL
 */
char *_getenv(char *name, PROGARGS *args)
{
	int i, nameLen = 0;

	if (name == NULL || args->envp == NULL)
		return (NULL);

	nameLen = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], nameLen) &&
				args->envp[i][nameLen] == '=')
		{
			return (args->envp[i] + nameLen + 1);
		}
	}
	return (NULL);
}

/**
 * cleanupAfterExecution - Free required memory
 *
 * @args: Arguments passed
 *
 * Return: Void
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
 * freeArgs - Free all memory
 *
 * @args: Arguments passed
 *
 * Return: Void
 */
void freeArgs(PROGARGS *args)
{
	if (args->fd != 0)
	{
		if (close(args->fd))
			perror("./hsh: ");
	}
	cleanupAfterExecution(args);
	freePtrs(args->envp);
}

/**
 * freePtrs - Free memory allocated to pointers
 *
 * @arr: Array of pointers
 *
 * Return: Void
 */
void freePtrs(char **arr)
{
	int i;

	if (arr)
	{
		for (i = 0; arr[i]; i++)
			free(arr[i]);

		free(arr);
		arr = NULL;
	}
}
