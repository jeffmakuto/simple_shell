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

	startShell(args, ac, av, envp);

	signal(SIGINT, handleCtrlCSignal);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && ac == 1)
	{
		errno = 2;
		prompt = PROMPT;
	}
	errno = 0;

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
	_print("\n");
	_print(PROMPT);
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
	int i = 0;

	args->programName = av[0];
	args->buffer = NULL;
	args->cmd = NULL;
	args->execCount = 0;

	if (ac == 1)
		args->fd = STDIN_FILENO;
	else
	{
		args->fd = open(av[1], O_RDONLY);
		if (args->fd == -1)
		{
			_printe(data->program_name);
			_printe(": 0: Can't open ");
			_printe(argv[1]);
			_printe("\n");
			exit(127);
		}
	}
	args->tokens = NULL;
	args->envp = malloc(sizeof(char *) * 10);
	if (envp)
	{
		for (; envp[i]; i++)
			args->envp[i] = _strdup(envp[i]);
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
	int errCode = 0, len = 0;

	while (++(args->execCount))
	{
		/* Primpt prompt to the user */
		_print(prompt);
		errCode = len = _getline(args);

		if (errCode == EOF)
		{
			freeArgs(args);
			exit(errno);
		}
		if (len >= 1)
		{
			replaceVariables(args);
			splitCommands(args);
			if (args->tokens[0])
			{
				errCode = executeCommand(args);
				if (errCode)
					printErr(errCode, args);
			}
			cleanupAfterExecution(args);
		}
	}
}
