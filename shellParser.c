#include "shell.h"

/**
 * handleSemiColonCommands - Splits a command string by semicolons
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
char **handleSemiColonCommands(const char *cmd, int *numCommands)
{
	char **commands = NULL, *token, *commandCopy = strdup(cmd);
	int i;

	if (!commandCopy)
	{
		perror("./hsh: strdup error");
		return (NULL);
	}
	commandCopy[strcspn(commandCopy, "\n")] = '\0'; /*Remove trailing newline */
	token = strtok(commandCopy, ";");
	while (token && *numCommands < MAX_ARGS)
	{
		commands = realloc(commands, sizeof(char *) * (*numCommands + 1));
		if (!commands)
		{
			perror("./hsh: realloc error");
			free(commandCopy);
			return (NULL);
		}
		commands[*numCommands] = strdup(token);
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
		token = strtok(NULL, ";");
	}
	free(commandCopy);
	return (commands);
}
/**
 * handleAndOperator - Handles the "&&" operator and executes the next command
 * only if the previous one succeeds (returns 0).
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handleAndOperator(char *cmd, char **envp)
{
	char *token = "&&", *nextCmd = NULL;
	int shouldExit = 0;

	nextCmd = strstr(cmd, token);
	if (nextCmd)
	{
		*nextCmd = '\0'; /* Null-terminate the first command */
		nextCmd += strlen(token); /* Move to the start of the next command */
		if (*nextCmd != '\0') /* Skip if there's nothing after && */
			shouldExit = processCommandInput(nextCmd, envp);
	}
	shouldExit = processSingleCommand(cmd, envp) || shouldExit;

	return (shouldExit);
}

/**
 * handleOrOperator - Handles the "||" operator and executes the next command
 * only if the previous one fails (returns non-zero).
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handleOrOperator(char *cmd, char **envp)
{
	char *token = "||";
	char *nextCmd = NULL;
	int shouldExit = 0;

	nextCmd = strstr(cmd, token);
	if (nextCmd)
	{
		*nextCmd = '\0'; /* Null-terminate the first command */
		nextCmd += strlen(token); /* Move to the start of the next command */

		if (*nextCmd != '\0') /* Skip if there's nothing after || */
		{
			shouldExit = processCommandInput(cmd, envp);
			if (!shouldExit) /* Only execute the next command if the previous one fails */
				shouldExit = handleOrOperator(nextCmd, envp);
		}
	}
	else
	{
		/* No || found, process the single command */
		shouldExit = processSingleCommand(cmd, envp);
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
	int numCommands = 0, shouldExit = 0, i;

	commands = handleSemiColonCommands(cmd, &numCommands);
	if (!commands)
		return (shouldExit); /* Return if there's an error splitting the commands */
	for (i = 0; i < numCommands; i++)
	{
		if (commands[i][0] != '\0') /* Skip empty commands */
		{
			/* Handle chaining with && and || */
			if (strstr(commands[i], "&&"))
				shouldExit = handleAndOperator(commands[i], envp);
			else if (strstr(commands[i], "||"))
				shouldExit = handleOrOperator(commands[i], envp);
			else
				shouldExit = processSingleCommand(commands[i], envp);
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

