#include "shell.h"

/**
 * processCommandLineArguments - processes command line arguments
 *
 * @ac: number of commandline arguments
 *
 * @av: array of command-line arguments
 *
 * @envp: array of environment variables
 *
 * Return: 0 on sucess, else 1 on failure
 */
int processCommandLineArguments(int ac, char **av, char **envp)
{
	char **args;

	if (ac > 1)
	{
		args = processCommand(av[1]);

		if (args)
		{
			executeCommand(args, envp);
			free(args);

			return (0);
		}
		else
			return (1);
	}
	return (1);
}

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
		write(STDOUT_FILENO, PROMPT, _strlen(PROMPT));
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
		{
			/* Null-terminate the command by replacing the newline character */
			cmd[bytesRead - 1] = '\0';
			shouldExit = processCommandInput(cmd, envp);
		}
	}
	free(cmd);
}

/**
 * main - entry point
 *
 * @ac: number of commandline arguments
 *
 * @av: array of strings of commandline arguments
 *
 * @envp: array of strings of environment variables
 *
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
	if (ac > 1)
	{
		if (processCommandLineArguments(ac, av, envp) == 0)
			return (0);
	}

	if (isatty(STDIN_FILENO))
		runInteractiveMode(envp);
	else
		runNonInteractiveMode(envp);

	return (0);
}
