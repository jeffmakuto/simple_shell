#include "shell.h"

/**
 * executeCommand - execute string of arguments
 *
 * @args: arguments passed
 *
 * Return: 0 on success, -1 on fail
 */
int executeCommand(PROGARGS *args)
{
	int result = 0, status;
	pid_t pid;

	result = checkBuiltins(args);
	if (result != -1)
		return (result);

	result = findExecutable(args);
	if (result)
		return (result);

	pid = fork();

	if (pid == -1)
	{
		perror(args->cmd);
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		result = execve(args->tokens[0], args->tokens, args->envp);
			if (result == -1)
			{
				perror(args->cmd);
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			errno = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			errno = 128 + WTERMSIG(status);
	}
	return (0);
}

/**
 * processCommand - processes an array of string commands into tokens
 *
 * @args: commands passed
 *
 * Return: tokens
 */
void processCommand(PROGARGS *args)
{
	char *delims = " \t";
	int i, j, count = 2, len;

	len = _strlen(args->buffer);
	if (len)
	{
		if (args->buffer[len - 1] == '\n')
			args->buffer[len - 1] = '\0';
	}

	for (i = 0; args->buffer[i]; i++)
	{
		for (j = 0; delims[j]; j++)
		{
			if (args->buffer[i] == delims[j])
				count++;
		}
	}

	args->tokens = malloc(count * sizeof(char *));
	if (args->tokens == NULL)
	{
		perror(args->progName);
		exit(errno);
	}
	i = 0;
	args->tokens[i] = _strdup(_strtok(args->buffer, delims));
	args->cmd = _strdup(args->tokens[0]);
	while (args->tokens[i++])
	{
		args->tokens[i] = _strdup(_strtok(NULL, delims));
	}
}

/**
 * getPath - tokenize path in directories
 *
 * @args: commands passed
 *
 * Return: array of path dirs
 */

char **getPath(PROGARGS *args)
{
	int i = 0;
	int countDirs = 2;
	char **tokens = NULL;
	char *PATH;

	PATH = _getenv("PATH", args);
	if ((PATH == NULL) || PATH[0] == '\0')
	{
		return (NULL);
	}

	PATH = _strdup(PATH);

	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			countDirs++;
	}

	tokens = malloc(sizeof(char *) * countDirs);

	i = 0;
	tokens[i] = _strdup(_strtok(PATH, ":"));
	while (tokens[i++])
	{
		tokens[i] = _strdup(_strtok(NULL, ":"));
	}

	free(PATH);
	PATH = NULL;
	return (tokens);

}

/**
 * checkFile - checks if a file exists, it's not a dir and
 * has excecution permisions for permisions.
 *
 * @filePath: pointer to full file name
 *
 * Return: 0 success, error code on fail
 */

int checkFile(char *filePath)
{
	struct stat statBuff;

	if (stat(filePath, &statBuff) != -1)
	{
		if (S_ISDIR(statBuff.st_mode) ||  access(filePath, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}
	errno = 127;
	return (127);
}

/**
 * replaceVariables - replaces specific variables in the input string with
 * their values
 *
 * @args: commands passed
 *
 * Return: void
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
