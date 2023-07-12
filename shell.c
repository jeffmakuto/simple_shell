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
	char *cmd = NULL, **args, *executable;
	size_t n = 0;
	ssize_t bytesRead;
	int i;

	if (ac > 1)
	{
		if (processCommandLineArguments(ac, av, envp) == 0)
			return 0;
	}
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		bytesRead = getline(&cmd, &n, stdin);

		if (bytesRead == -1)
		{
			free(cmd);
			perror("getline error");
			exit(EXIT_FAILURE);
		}

		if (*cmd != '\n')
		{
			cmd[strcspn(cmd, "\n")] = '\0'; /* Remove trailing newline */
			args = processCommand(cmd);

			if (args)
			{
				if (strcmp("exit", args[0]) == 0)
				{
					free(args[0]);
					free(args);
					break;
				}
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
	}
	free(cmd);
	return 0;
}

