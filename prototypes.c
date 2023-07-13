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
		perror("malloc error");
		return (NULL);
	}

	token = strtok(cmd, " ");

	while (token && argCount < MAX_ARGS)
	{
		args[argCount] = strdup(token);

		if (!args[argCount])
		{
			perror("strdup error");

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
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return;
	}
	if (pid == 0)
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror("./hsh: No such file or directory\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
}

/**
 * checkBuiltins - checks if a command is a built-in one and
 * performs associated action
 *
 * @cmd: the command to be checked
 *
 * Return: 1 if the command is a built-in one else 0
 */
int checkBuiltins(char *cmd, char **args)
{
	int i;

	BuiltinCmd builtins[] =
	{
		{"cd", cdAction},
		{NULL, NULL}
	};

	i = 0;

	while (builtins[i].cmd)
	{
		if (strcmp(cmd, builtins[i].cmd) == 0)
		{
			if (builtins[i].action)
				builtins[i].action(args);
			return (1); /* it's a built-in command */
		}
		i++;
	}
	return (0); /* not a built-in command */
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
 * findExecutable - Searches for the executable in the system's PATH environment variable.
 *
 * @cmd: The command to find.
 *
 * @envp: The environment variables.
 *
 * Return: A pointer to the executable path if found, NULL otherwise.
 */
char *findExecutable(const char *cmd)
{
	char *path = getenv("PATH"), *pathEnv, *dir, *executablePath;
	size_t dirLen, cmdLen;
	char *foundExecutable = NULL;

	if (!path)
	{
		perror("PATH environment variable not found");
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
			perror("Memory allocation failed");
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
	else
	{
		perror("Command not found");
		return (NULL);
	}
}

