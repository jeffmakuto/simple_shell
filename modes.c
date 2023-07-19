#include "shell.h"

/**
 * runInteractiveMode - Runs the shell in interactive mode.
 *
 * @envp: The environment variables.
 *
 * Return: Void
 */
void runInteractiveMode(char **envp)
{
	char *cmd = NULL;
	size_t n = 0;
	ssize_t bytesRead;
	int shouldExit = 0;

	while (!shouldExit)
	{
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		bytesRead = getline(&cmd, &n, stdin);

		if (bytesRead == -1)
		{
			free(cmd);
			perror("getline error");
			exit(EXIT_FAILURE);
		}

		if (*cmd != '\n')
			shouldExit = processCommandInput(cmd, envp);
	}
	free(cmd);
}

/**
 * runNonInteractiveMode - Runs the shell in non-interactive mode.
 *
 * @envp: The environment variables.
 *
 * Return: Void
 */
void runNonInteractiveMode(char **envp)
{
	char *cmd = NULL;
	size_t n = 0;
	ssize_t bytesRead;
	int shouldExit = 0;

	while (!shouldExit && (bytesRead = getline(&cmd, &n, stdin)) != -1)
	{
		if (*cmd != '\n')
			shouldExit = processCommandInput(cmd, envp);
	}
	free(cmd);
}


/**
 * processCommandInput - Process the command input
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int processCommandInput(char *cmd, char **envp)
{
	char **commands = NULL, *token;
	int i, numCommands = 0, shouldExit = 0;

	cmd[strcspn(cmd, "\n")] = '\0'; /* Remove trailing newline */
	token = strtok(cmd, ";");
	while (token && numCommands < MAX_ARGS)
	{
		commands = realloc(commands, sizeof(char *) * (numCommands + 1));
		if (!commands)
		{
			perror("./hsh: realloc error");
			exit(EXIT_FAILURE);
		}
		commands[numCommands] = strdup(token);
		if (!commands[numCommands])
		{
			perror("./hsh: strdup error");
			/* Free previously allocated strings */
			for (i = 0; i < numCommands; i++)
				free(commands[i]);
			free(commands);
			exit(EXIT_FAILURE);
		}
		numCommands++;
		token = strtok(NULL, ";");
	}
	for (i = 0; i < numCommands; i++)
	{
		if (commands[i][0] != '\0') /* Skip empty commands */
			shouldExit = shouldExit || processSingleCommand(commands[i], envp);
		free(commands[i]);
	}
	free(commands);
	return (shouldExit);
}

/**
 * processSingleCommand - Process a single command
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int processSingleCommand(char *cmd, char **envp)
{
	char **args, *executable;
	int shouldExit = 0, i;

	args = processCommand(cmd);
	if (args)
	{
		if (strcmp("exit", args[0]) == 0)
		{
			free(args[0]);
			shouldExit = 1;
			/* If an argument is provided to the exit command */
			if (args[1])
				exit(atoi(args[1]));
			else
				exit(0);
		}
		else
		{
			if (!checkBuiltins(args[0], args))
			{
				executable = findExecutable(args[0]);
				if (executable)
				{
					args[0] = executable;
					executeCommand(args, envp);
				}
			}
			for (i = 0; args[i]; i++)
				free(args[i]);
			free(args);
		}
	}
	return (shouldExit);
}

