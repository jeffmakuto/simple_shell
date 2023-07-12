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
