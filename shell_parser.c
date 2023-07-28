#include "shell.h"

/**
 * handle_semi_coloned_commands - Splits a command string by semicolons
 * into individual commands.
 *
 * @cmd: The command string to be split.
 *
 * @num_commands: A pointer to an integer that will store the number
 * of commands found.
 *
 * Return: An array of strings, each representing an individual command.
 * The last element will be NULL. NULL will be returned in case of an error.
 */
char **handle_semi_coloned_commands(char *cmd, int *num_commands)
{
	char **commands = NULL, *token, *command_copy = _strdup((char *)cmd);
	int i;

	if (!command_copy)
	{
		perror("./hsh: strdup error");
		return (NULL);
	}
	command_copy[_strcspn(command_copy, "\n")] = '\0'; /*Remove trailing newline */
	token = _strtok(command_copy, ";");
	while (token && *num_commands < MAX_ARGS)
	{
		commands = _realloc(commands, sizeof(char *) * (*num_commands + 1));
		if (!commands)
		{
			perror("./hsh: realloc error");
			free(command_copy);
			return (NULL);
		}
		commands[*num_commands] = _strdup(token);
		if (!commands[*num_commands])
		{
			perror("./hsh: strdup error");
			/* Free previously allocated strings */
			for (i = 0; i < *num_commands; i++)
				free(commands[i]);
			free(commands);
			free(command_copy);
			return (NULL);
		}
		(*num_commands)++;
		token = _strtok(NULL, ";");
	}
	free(command_copy);
	return (commands);
}

/**
 * handle_and_operator - Handle command execution with the && operator.
 *
 * @cmd: The command string with && operator.
 *
 * @envp: The environment variables.
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handle_and_operator(char *cmd, char **envp)
{
	char *token, *saveptr;
	int should_exit = 0, last_exit_status = 0;

	token = _strtok_r(cmd, "&&", &saveptr);
	while (token && !should_exit)
	{
		if (token[0]) /* Skip empty commands */
		{
			should_exit = process_single_command(token, envp, &last_exit_status);
			if (should_exit)
				break; /* Stop executing if a command fails */
		}
		token = _strtok_r(NULL, "&&", &saveptr);
	}

	return (should_exit);
}

/**
 * handle_or_operator - Handle command execution with the || operator.
 *
 * @cmd: The command string with || operator.
 *
 * @envp: The environment variables.
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int handle_or_operator(char *cmd, char **envp)
{
	char *token, *saveptr;
	int should_exit = 0, last_exit_status = 0;

	token = _strtok_r(cmd, "||", &saveptr);
	while (token && !should_exit)
	{
		if (token[0]) /* Skip empty commands */
		{
			should_exit = process_single_command(token, envp, &last_exit_status);
			if (!should_exit)
				break; /* Stop executing if a command succeeds */
		}
		token = _strtok_r(NULL, "||", &saveptr);
	}

	return (should_exit);
}

/**
 * process_command_nput - Process the command input
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int process_command_input(char *cmd, char **envp)
{
	char **commands = NULL;
	int num_commands = 0, should_exit = 0, i, last_exit_status = 0;

	commands = handle_semi_coloned_commands(cmd, &num_commands);
	if (!commands)
		return (should_exit); /* Return if there's an error splitting the commands */
	for (i = 0; i < num_commands; i++)
	{
		if (commands[i][0] != '\0') /* Skip empty commands */
		{
			/* Handle chaining with && and || */
			if (_strstr(commands[i], "&&"))
				should_exit = handle_and_operator(commands[i], envp);
			else if (_strstr(commands[i], "||"))
				should_exit = handle_or_operator(commands[i], envp);
			else
				should_exit = process_single_command(commands[i], envp, &last_exit_status);
		}
		free(commands[i]);
	}
	free(commands);
	return (should_exit);
}

/**
 * process_single_command - Process a single command
 *
 * @cmd: The command string
 *
 * @envp: The environment variables
 *
 * @last_exit_status_ptr: Pointer to an integer to store the exit status
 * of the last command
 *
 * Return: 1 if the program should exit, 0 otherwise.
 */
int process_single_command(char *cmd, char **envp, int *last_exit_status_ptr)
{
	char **args, *executable, *replaced_cmd;
	int should_exit = 0, status;

	/* Replace variables before processing the command */
	replaced_cmd = replace_variables(cmd, envp);
	args = process_command(replaced_cmd);

	if (args)
	{
		if (_strcmp("exit", args[0]) == 0)
		{
			free(args[0]);
			should_exit = 1;
			/* If an argument is provided to the exit command */
			if (args[1])
			{
				exit(_atoi(args[1]));
			}
			else
				exit(0);
		}
		else
		{
			if (!check_builtins(args[0], args, envp))
			{
				executable = find_executable(args[0], envp);
				if (executable)
				{
					args[0] = executable;
					execute_command(args, envp);
					wait(&status);
					*last_exit_status_ptr = WEXITSTATUS(status);
				}
			}
			free_args(args);
		}
	}
	free(replaced_cmd);
	return (should_exit);
}

