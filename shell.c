#include "shell.h"

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

