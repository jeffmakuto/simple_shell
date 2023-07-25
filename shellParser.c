#include "shell.h"

/**
 * handleSemiColonedCommands - Splits a command string by semicolons
 * into individual commands.
 *
 * @cmd: The command string to be split.
 *
 * @numCommands: A pointer to an integer that will store the number
 * of commands found.
 *
 * Return: An array of strings, each representing an individual command.
 * The last element will be NULL. NULL will be returned in case of an error.
 */
char **handleSemiColonedCommands(char *cmd, int *numCommands)
{
	char **commands = NULL, *token, *commandCopy = _strdup((char *)cmd);
	int i;

	if (!commandCopy)
	{
		perror("./hsh: strdup error");
		return (NULL);
	}
	commandCopy[strcspn(commandCopy, "\n")] = '\0'; /*Remove trailing newline */
	token = _strtok(commandCopy, ";");
	while (token && *numCommands < MAX_ARGS)
	{
		commands = realloc(commands, sizeof(char *) * (*numCommands + 1));
		if (!commands)
		{
			perror("./hsh: realloc error");
			free(commandCopy);
			return (NULL);
		}
		commands[*numCommands] = _strdup(token);
		if (!commands[*numCommands])
		{
			perror("./hsh: strdup error");
			/* Free previously allocated strings */
			for (i = 0; i < *numCommands; i++)
				free(commands[i]);
			free(commands);
			free(commandCopy);
			return (NULL);
		}
		(*numCommands)++;
		token = _strtok(NULL, ";");
	}
	free(commandCopy);
	return (commands);
}

/**
 * handleAndOperator - Handle command execution with the && operator.
 *
 * @cmd: The command string with && operator.
 *
 * @envp: The environment variables.
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handleAndOperator(char *cmd, char **envp)
{
	char *token, *saveptr;
	int shouldExit = 0, lastExitStatus = 0;

	token = strtok_r(cmd, "&&", &saveptr);
	while (token && !shouldExit)
	{
		if (token[0]) /* Skip empty commands */
		{
			shouldExit = processSingleCommand(token, envp, &lastExitStatus);
			if (shouldExit)
				break; /* Stop executing if a command fails */
		}
		token = strtok_r(NULL, "&&", &saveptr);
	}

	return (shouldExit);
}

/**
 * handleOrOperator - Handle command execution with the || operator.
 *
 * @cmd: The command string with || operator.
 *
 * @envp: The environment variables.
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handleOrOperator(char *cmd, char **envp)
{
	char *token, *saveptr;
	int shouldExit = 0, lastExitStatus = 0;

	token = strtok_r(cmd, "||", &saveptr);
	while (token && !shouldExit)
	{
		if (token[0]) /* Skip empty commands */
		{
			shouldExit = processSingleCommand(token, envp, &lastExitStatus);
			if (!shouldExit)
				break; /* Stop executing if a command succeeds */
		}
		token = strtok_r(NULL, "||", &saveptr);
	}

	return (shouldExit);
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
	char **commands = NULL;
	int numCommands = 0, shouldExit = 0, i, lastExitStatus = 0;

	commands = handleSemiColonedCommands(cmd, &numCommands);
	if (!commands)
		return (shouldExit); /* Return if there's an error splitting the commands */
	for (i = 0; i < numCommands; i++)
	{
		if (commands[i][0] != '\0') /* Skip empty commands */
		{
			/* Handle chaining with && and || */
			if (_strstr(commands[i], "&&"))
				shouldExit = handleAndOperator(commands[i], envp);
			else if (_strstr(commands[i], "||"))
				shouldExit = handleOrOperator(commands[i], envp);
			else
				shouldExit = processSingleCommand(commands[i], envp, &lastExitStatus);
		}
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
 * @lastExitStatusPtr: Pointer to an integer to store the exit status
 * of the last command
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int processSingleCommand(char *cmd, char **envp, int *lastExitStatusPtr)
{
	char **args, *executable;
	int shouldExit = 0, i, status;

	/* Replace variables before processing the command */
	cmd = replaceVariables(cmd);

	args = processCommand(cmd);
	if (args)
	{
		if (_strcmp("exit", args[0]) == 0)
		{
			free(args[0]);
			shouldExit = 1;
			/* If an argument is provided to the exit command */
			if (args[1])
				exit(_atoi(args[1]));
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
					wait(&status);
					*lastExitStatusPtr = WEXITSTATUS(status);
				}
			}
			for (i = 0; args[i]; i++)
				free(args[i]);
			free(args);
		}
	}
	return (shouldExit);
}

