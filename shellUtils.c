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
int executeCommand(PROGARGS *args, int *exitStatus, int *termSig)
{
	int result = 0, status;
	pid_t pid;

	result = checkBuiltins(args);
	if (result != -1)
	{
		*exitStatus = result;
		*termSig = 0;
		return (0);
	}
	result = findExecutable(args);
	if (result)
	{
		return (result);
	}
	else
	{
		pid = fork();
		if (pid < 0)
		{
			perror("./hsh: fork error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			result = execve(args->tokens[0], args->tokens, args->envp);
			if (result == -1)
			{
				perror("./hsh: execve error");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{
				*exitStatus = WEXITSTATUS(status);
				*termSig = 0;
			}
			else if (WIFSIGNALED(status))
			{
				*exitStatus = WTERMSIG(status);
				*termSig = 1;
			}
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
		return (-1);
	if (args->cmd[0] == '/' || args->cmd[0] == '.')
		return (checkFile(args->cmd));
	free(args->tokens[0]);
	args->tokens[0] = _strcat(_strdup("/"), args->cmd);
	if (!args->tokens[0])
		return (-1);
	dirs = getPath(args);
	if (!dirs || !dirs[0])
	{
		perror("./hsh: Dir Errror");
		return (-1);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], args->tokens[0]);
		result = checkFile(dirs[i]);
		if (!result)/* If file's found and executable.*/
		{
			free(args->tokens[0]);
			args->tokens[0] = _strdup(dirs[i]);
			freePtrs(dirs);
			return (0);
		}
		else if (result == -1 && errno == EACCES)/*If file's non-executable*/
		{
			perror("./hsh: Permission denied");
			free(args->tokens[0]);
			args->tokens[0] = NULL;
			freePtrs(dirs);
			return (-1);
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
			perror("./hsh: Error: Not an executable file");
			return (-1);
		}
		return (0);
	}
	perror("./hsh: Error: File not found");
	return (-1);
}

/**
 * splitCommands - this function separates the string using a designed delimiter
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
			data->buffer[len - 1] = '\0';
	}

	for (i = 0; data->buffer[i]; i++)
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
		perror("./hsh: Memory Allocation failure");
		exit(EXIT_FAILURE);
	}
	i = 0;
	args->tokens[i] = _strdup(_strtok(args->buffer, delims));
	args->cmd = _strdup(args->tokens[0]);
	while (args->tokens[i++])
	{
		args->tokens[i] = _strdup(_strtok(NULL, delims));
	}
}
