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

	while (1)
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
			processCommandInput(cmd, envp);
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

	while ((bytesRead = getline(&cmd, &n, stdin)) != -1)
	{
		if (*cmd != '\n')
			processCommandInput(cmd, envp);
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
 * Return: Void
 */
void processCommandInput(char *cmd, char **envp)
{
	char **args, *executable;
	int i;

	cmd[strcspn(cmd, "\n")] = '\0'; /* Remove trailing newline */
	args = processCommand(cmd);

	if (args)
	{
		if (strcmp("exit", args[0]) == 0)
		{
			free(args[0]);
			free(args);
			return;
		}

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

