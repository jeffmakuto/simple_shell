#include "shell.h"

/**
 * process_command_line_arguments - processes command line arguments
 *
 * @ac: number of commandline arguments
 *
 * @av: array of command-line arguments
 *
 * @envp: array of environment variables
 *
 * Return: 0 on sucess, else 1 on failure
 */
int process_command_line_arguments(int ac, char **av, char **envp)
{
	char **args;

	if (ac > 1)
	{
		args = process_command(av[1]);

		if (args)
		{
			execute_command(args, envp);
			free(args);

			return (0);
		}
		else
			return (1);
	}
	return (1);
}

/**
 * run_interactive_mode - Runs the shell in interactive mode.
 *
 * @envp: The environment variables.
 *
 * Return: Void
 */
void run_interactive_mode(char **envp)
{
	char *cmd = NULL;
	size_t n = 0;
	ssize_t bytes_read;
	int should_exit = 0;

	while (!should_exit)
	{
		write(STDOUT_FILENO, PROMPT, _strlen(PROMPT));
		bytes_read = getline(&cmd, &n, stdin);

		if (bytes_read == -1)
		{
			free(cmd);
			perror("getline error");
			exit(EXIT_FAILURE);
		}

		if (*cmd != '\n')
			should_exit = process_command_input(cmd, envp);
	}
	free(cmd);
}

/**
 * run_non_interactive_mode - Runs the shell in non-interactive mode.
 *
 * @envp: The environment variables.
 *
 * Return: Void
 */
void run_non_interactive_mode(char **envp)
{
	char *cmd = NULL;
	size_t n = 0;
	ssize_t bytes_read;
	int should_exit = 0;

	while (!should_exit && (bytes_read = getline(&cmd, &n, stdin)) != -1)
	{
		if (*cmd != '\n')
		{
			/* Null-terminate the command by replacing the newline character */
			cmd[bytes_read - 1] = '\0';
			should_exit = process_command_input(cmd, envp);
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
		if (process_command_line_arguments(ac, av, envp) == 0)
			return (0);
	}

	if (isatty(STDIN_FILENO))
		run_interactive_mode(envp);
	else
		run_non_interactive_mode(envp);

	return (0);
}


