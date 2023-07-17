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
			perror("./hsh: getline error");
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

	while ((bytesRead = getline(&cmd, &n, stdin)) != -1)
	{
		if (*cmd != '\n')
		{
			cmd[bytesRead - 1] = '\0'; /* Remove trailing newline */
			executeMultipleCommands(cmd, envp);
		}
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
	char **args, *executable;
	int i, shouldExit = 0;

	cmd[strcspn(cmd, "\n")] = '\0'; /* Remove trailing newline */
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
			expandVariables(args, envp);

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
 * expandVariables - Expands variables in command arguments.
 *
 * @args: The array of command arguments.
 *
 * @envp: Array of strings of environment variables.
 *
 * This function checks each argument in the args array for variables
 * (strings starting with '$') and replaces them with their corresponding
 * values from the environment.
 */
void expandVariables(char **args, char **envp)
{
	char *variable, *value;
	int i;

	for (i = 0; args[i]; i++)
	{
		if (args[i][0] == '$')
		{
			variable = args[i] + 1; /* Skip the '$' sign */
			value = replaceVariableWithValue(variable, envp);

			if (value)
			{
				free(args[i]);
				args[i] = value;
			}
			else
			{
				/* If the variable is not found in the environment,
				 * replace it with an empty string
				 */
				free(args[i]);
				args[i] = strdup("");
			}
		}
	}
}

/**
 * replaceVariableWithValue - Replaces variables with their values from envp.
 *
 * @variable: The variable name to be replaced (without the '$' sign).
 * @envp: Array of strings of environment variables.
 *
 * Return: A newly allocated string containing the value of the variable.
 * The caller is responsible for freeing the memory.
 */
char *replaceVariableWithValue(const char *variable, char **envp)
{
	char *value = NULL;
	int i;

	for (i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], variable, strlen(variable)) == 0 &&
				envp[i][strlen(variable)] == '=')
		{
			value = strdup(envp[i] + strlen(variable) + 1);
			break;
		}
	}
	return (value);
}

