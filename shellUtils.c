#include "shell.h"

/**
 * processCommand - processes an array of string commands
 *
 * @cmd: the commands to be processed
 *
 * Return: array of string commands
 */
char **processCommand(char *cmd)
{
	char **args = malloc(sizeof(char *) * MAX_ARGS), *token;
	int argCount = 0;

	if (!args)
	{
		perror("./hsh: malloc error");
		return (NULL);
	}
	/* Ignore anything after the '#' character (comment handling) */
	token = _strtok(cmd, "#");
	if (!token)
	{
		free(args);
		return (NULL);
	}
	token = _strtok(cmd, " ");
	while (token && argCount < MAX_ARGS)
	{
		args[argCount] = _strdup(token);

		if (!args[argCount])
		{
			perror("./hsh: strdup error");
			/* Free previously allocated strings */
			freeArgs(args);
			return (NULL);
		}
		argCount++;

		if (_strchr(token, '\n'))
		{
			args[argCount] = NULL;
			break; /* Exit the loop after setting NULL terminator */
		}
		token = _strtok(NULL, " ");
	}
	args[argCount] = NULL;
	return (args);
}

/**
 * executeCommand - execute string of arguments
 *
 * @args: string of arguments to be executed
 *
 * @envp: environment variable arguments
 *
 * Return: Void
 */
void executeCommand(char **args, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("./hsh: fork error");
		return;
	}
	if (pid == 0)
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror("./hsh: execve error");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
}

/**
 * isAbsolutePath - Checks if the given path is an absolute path.
 *
 * @path: The path to check.
 *
 * Return: true if the path is an absolute path and executable,
 * false otherwise.
 */
bool isAbsolutePath(char *path)
{
	if (path[0] == '/')
	{
		if (isExecutable(path))
			return (true);
		perror("./hsh: ");
		return (false);
	}
	return (false);
}

/**
 * isExecutable - Checks if the given path is executable.
 *
 * @path: The path to check.
 *
 * Return: 1 if the path is executable, 0 otherwise.
 */
int isExecutable(char *path)
{
	/* Check if the given path is executable */
	return (access(path, X_OK) == 0);
}

/**
 * findExecutable - Searches for the executable in the system's
 * PATH environment variable.
 *
 * @cmd: The command to find.
 *
 * @envp: The environment variables
 *
 * Return: A pointer to the executable path if found, NULL otherwise.
 */
char *findExecutable(char *cmd, char **envp)
{
	char *path = _getenv("PATH", envp), *pathEnv, *dir, *executablePath;
	size_t dirLen, cmdLen;
	char *foundExecutable = NULL;

	if (isAbsolutePath(cmd))
		return (_strdup(cmd));
	if (!path)
	{
		perror("./hsh: PATH environment variable not found");
		return (NULL);
	}
	pathEnv = _strdup(path);
	dir = _strtok(pathEnv, ":");
	while (dir)
	{
		dirLen = _strlen(dir);
		cmdLen = _strlen(cmd);
		executablePath = malloc(dirLen + cmdLen + 2);
		if (!executablePath)
		{
			perror("./hsh: Memory allocation failed");
			free(pathEnv);
			return (NULL);
		}
		_memcpy(executablePath, dir, dirLen);
		executablePath[dirLen] = '/';
		_memcpy(executablePath + dirLen + 1, cmd, cmdLen + 1);
		if (isExecutable(executablePath))
		{
			foundExecutable = executablePath;
			break;
		}
		free(executablePath);
		dir = _strtok(NULL, ":");
	}
	free(pathEnv);
	if (foundExecutable)
		return (foundExecutable);
	perror("./hsh: ");
	return (NULL);
}
