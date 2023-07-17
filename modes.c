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
	char **args, *executable, *semicolonPos;
	int i, shouldExit = 0;

	cmd[strcspn(cmd, "\n")] = '\0'; /* Remove trailing newline */

	/* Check if the command contains a semicolon to execute multiple commands */
	semicolonPos = strchr(cmd, ';');
	if (semicolonPos)
		return (executeMultipleCommands(cmd, envp));
	/* If there's no semicolon, process the single command as before */
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

/**
 * executeMultipleCommands - Execute multiple commands separated by semicolons.
 *
 * @commands: The command string containing multiple commands separated
 * by semicolons.
 *
 * @envp: Array of strings representing the environment variables.
 *
 * Return: Exit status of the last executed command.
 */
int executeMultipleCommands(char *commands, char **envp)
{
	int lastCommandExitStatus = 0, i, commandExitStatus;
	char *command, **args;
	char *saveptr; /* For strtok_r, thread-safe version of strtok */

	command = strtok_r(commands, ";", &saveptr);
	while (command)
	{
		args = processCommand(command);
		if (args)
		{
			executeCommand(args, envp);
			/* Wait for the child process to complete and get its exit status. */
			wait(&commandExitStatus);
			/* Check if the child process exited normally. */
			if (WIFEXITED(commandExitStatus))
				commandExitStatus = WEXITSTATUS(commandExitStatus);
			else
				commandExitStatus = 1; /* Set non-zero exit status in case of abnormal termination. */
			lastCommandExitStatus = commandExitStatus;
			/* Free the allocated memory for the arguments. */
			for (i = 0; args[i]; i++)
				free(args[i]);
			free(args);
		}
		command = strtok_r(NULL, ";", &saveptr);
	}
	return (lastCommandExitStatus);
}
