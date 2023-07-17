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
	char **args = malloc(sizeof(char *) * MAX_ARGS);
	char *token;
	int argCount = 0, i;

	if (!args)
	{
		perror("./hsh: malloc error");
		return (NULL);
	}

	token = strtok(cmd, " ");

	while (token && argCount < MAX_ARGS)
	{
		args[argCount] = strdup(token);

		if (!args[argCount])
		{
			perror("./hsh: strdup error");

			/* Free previously allocated strings */
			for (i = 0; i < argCount; i++)
				free(args[i]);
			free(args);
			return (NULL);
		}
		argCount++;

		if (strchr(token, '\n'))
		{
			args[argCount] = NULL;
			break; /* Exit the loop after setting NULL terminator */
		}

		token = strtok(NULL, " ");
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
	int status, i;

	handleIORedirection(args);

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

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
}

/**
 * isAbsolutePath - Checks if the given path is an absolute path.
 *
 * @path: The path to check.
 *
 * Return: true if the path is an absolute path and executable,
 * false otherwise.
 */
bool isAbsolutePath(const char *path)
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
int isExecutable(const char *path)
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
 * Return: A pointer to the executable path if found, NULL otherwise.
 */
char *findExecutable(const char *cmd)
{
	char *path = getenv("PATH"), *pathEnv, *dir, *executablePath;
	size_t dirLen, cmdLen;
	char *foundExecutable = NULL;

	if (isAbsolutePath(cmd))
		return (strdup(cmd));
	if (!path)
	{
		perror("./hsh: PATH environment variable not found");
		return (NULL);
	}
	pathEnv = strdup(path);
	dir = strtok(pathEnv, ":");
	while (dir)
	{
		dirLen = strlen(dir);
		cmdLen = strlen(cmd);
		executablePath = malloc(dirLen + cmdLen + 2);
		if (!executablePath)
		{
			perror("./hsh: Memory allocation failed");
			free(pathEnv);
			return (NULL);
		}
		memcpy(executablePath, dir, dirLen);
		executablePath[dirLen] = '/';
		memcpy(executablePath + dirLen + 1, cmd, cmdLen + 1);
		if (isExecutable(executablePath))
		{
			foundExecutable = executablePath;
			break;
		}
		free(executablePath);
		dir = strtok(NULL, ":");
	}
	free(pathEnv);
	if (foundExecutable)
		return (foundExecutable);
	perror("./hsh: ");
	return (NULL);
}

