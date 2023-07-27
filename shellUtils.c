#include "shell.h"

/**
 * executeCommand - Executes string of arguments
 *
 * @args: Pointer to program arguments struct
 *
 * @exitStatus: Pointer to an integer to store the exit status of
 * the child process
 *
 * @termSig: Pointer to an integer to indicate
 * if the child process was terminated by a signal
 *
 * Return: 0 on success, -1 on fail
 */
int executeCommand(PROGARGS *args)
{
	int result = 0, status;
	pid_t pid;

	result = checkBuiltins(args);
	if (result != -1)
		return(result);
	result = findExecutable(args);
	if (result)
		return (result);
	else
	{
		pid = fork();
		if (pid < 0)
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
	}
	return (0);
}

/**
 * findExecutable - look for a program in path
 *
 * @args: Ptr to program arguments
 *
 * Return: 0 success, error code on fail
 */
int findExecutable(PROGARGS *args)
{
	int i = 0, result = 0;
	char **dirs;

	if (!args->cmd)
		return (2);
	if (args->cmd[0] == '/' || args->cmd[0] == '.')
		return (checkFile(args->cmd));
	free(args->tokens[0]);
	args->tokens[0] = _strcat(_strdup("/"), args->cmd);
	if (!args->tokens[0])
		return (-1);
	dirs = getPath(args);
	if (!dirs || !dirs[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], args->tokens[0]);
		result = checkFile(dirs[i]);
		if (result == 0 || result == 126)
		{
			errno = 0;
			free(args->tokens[0]);
			args->tokens[0] = _strdup(dirs[i]);
			freePtrs(dirs);
			return (result);
		}
	}
	free(args->tokens[0]);
	args->tokens[0] = NULL;
	freePtrs(dirs);
	return (result);
}

/**
 * getPath - Tokenize path in dirs
 *
 * @args: Ptr to program arguments
 *
 * Return: An array of path dirs
 */

char **getPath(PROGARGS *args)
{
	int i = 0;
	int countDirs = 2;
	char **tokens = NULL;
	char *PATH;

	PATH = _getenv("PATH", args);
	if ((PATH) == 0 || PATH[0] == '\0')
		return (NULL);

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
 * checkFile - Checks if a file exists, it's not a dir and
 * has excecution permisions for permisions.
 *
 * @filePath: Ptr to full file name
 *
 * Return: 0 success, error code on fail
 */

int checkFile(char *filePath)
{
	struct stat fileStat;

	if (stat(filePath, &fileStat) != -1)
	{
		if (S_ISDIR(fileStat.st_mode) ||  access(filePath, X_OK))
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
 * splitCommands - this function separates string using a designed delimiter
 *
 * @args: Pointer to the program's arguments
 *
 * Return: Array of strings
 */
void splitCommands(PROGARGS *args)
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
		perror(args->programName);
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
