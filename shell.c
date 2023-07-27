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

int main(int ac, char *av[], char *envp[])
{
	PROGARGS ShellArgs = {NULL}, *args = &ShellArgs;
	char *prompt = NULL;
	bool hasError = false;

	startShell(args, ac, av, envp);

	signal(SIGINT, handleCtrlC);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && ac == 1)
	{
		hasError = true;
		prompt = PROMPT;
	}
	hasError = false;
	runShell(prompt, args);

	return (0);
}

/**
 * handleCtrlCSignal - Handles the Ctrl+C (SIGINT) signal.
 *
 * @signal: The signal number (unused).
 *
 */
void handleCtrlCSignal(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	write(STDOUT_FILENO, PROMPT, _strlen(PROMPT));
}

/**
 * startShell - Initialize struct with the data for the program
 *
 * @args: Ptr to data structure
 *
 * @ac: Number of arguments
 *
 * @av: Array of arguments
 *
 * @envp: The environemnt variables
 *
 * Return: Void
 */
void startShell(PROGARGS *args, int ac, char *av[], char *envp[])
{
	int i = 0, envpCap = INITIAL_ENVP_SIZE;

	args->programName = argv[0];
	args->buffer = NULL;
	args->cmd = NULL;
	args->execCount = 0;

	if (argc == 1)
		args->fd = STDIN_FILENO;
	else
	{
		args->fd = open(argv[1], O_RDONLY);
		if (args->fd == -1)
		{
			perror("./hsh: Error opening file");
			exit(EXIT_FAILURE);
		}
	}
	args->tokens = NULL;
	args->envp = malloc(sizeof(char *) * 10);
	if (envp)
	{
		for (; envp[i]; i++)
		{
			if (i >= envpCap)
			{
				envpCap *= 2;
				args->envp = _realloc(args->envp, sizeof(char *) * envCap);
			}
		}
	}
	args->envp[i] = NULL;
	envp = args->envp;
}

/**
 * runShell - loops the prompt
 *
 * @prompt: prompt
 *
 * @args: infinite loop for prompt
 *
 * Return: Void
 */
void runShell(char *prompt, PROGARGS *args)
{
	int len = 0, execRes;

	while (1)
	{
		args->execCount++;

		/* Primpt prompt to the user */
		write(STDOUT_FILENO, PROMPT, _strlen(PROMPT));
		len = _getline(args);

		if (len == EOF)
		{
			freeArgs(args);
			exit(EXIT_FAILURE);
		}
		if (len >= 1)
		{
			replaceVariables(args);
			splitCommands(args);
			if (args->tokens[0])
			{
				execRes = executeCommand(args);
				if (execRes)
					perror("./hsh: Execution error");
			}
			cleanupAfterExecution(args);
		}
	}
}
