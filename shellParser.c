#include "shell.h"

/**
 * executeSingleCommand - Execute a single command without any
 * command separator.
 *
 * @cmd: The command to execute.
 *
 * @envp: The environment variables.
 *
 * Return: Void.
 */
void executeSingleCommand(char *cmd, char **envp)
{
	int shouldExit;

	shouldExit = processCommandInput(cmd, envp);

	if (shouldExit)
		exit(EXIT_SUCCESS);
}

/**
 * executeMultipleCommands - Execute multiple commands separated by ';'.
 *
 * @cmds: The commands to execute.
 *
 * @envp: The environment variables.
 *
 * Return: Void.
 */
void executeMultipleCommands(char *args, char **envp)
{
	char *command;
	char *args_copy = strdup(args); /* Create a copy to avoid modifying the original string */

	while ((command = strsep(&args_copy, ";")))
	{
		if (*command != '\0')
			executeSingleCommand(command, envp);
	}
	free(args_copy);
}

