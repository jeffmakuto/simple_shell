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
	PROGARGS shellArgs = {NULL}, *args = &shellArgs;
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
 * handleCtrlCSignal - Handles Cntrl + C
 *
 * @signal: unused parameter.(Signal)
 *
 * Return: void
 */
void handleCtrlCSignal(int signal)
{
	(void)signal;
	_print("\n");
	_print(PROMPT);
}

/**
 * startShell - launches the shell
 *
 * @args: commands  passed
 *
 * @ac: argument count
 *
 * @av: argument vector
 *
 * @envp: environment varibles
 *
 * Return: void
 *
 */
void startShell(PROGARGS *args, int ac, char *av[], char **envp)
{
	int i = 0;

	args->progName = av[0];
	args->buffer = NULL;
	args->cmd = NULL;
	args->count = 0;

	if (ac == 1)
		args->fd = STDIN_FILENO;
	else
	{
		args->fd = open(av[1], O_RDONLY);
		if (args->fd == -1)
		{
			_printe(args->progName);
			_printe("open error");
			_printe(av[1]);
			_printe("\n");
			exit(127);
		}
	}
	args->tokens = NULL;
	args->envp = malloc(sizeof(char *) * MAX_PATH_LEN);
	if (envp)
	{
		for (; envp[i]; i++)
		{
			args->envp[i] = _strdup(envp[i]);
		}
	}
	args->envp[i] = NULL;
	envp = args->envp;

	args->aliasList = malloc(sizeof(char *) * INITIAL_ENVP_SIZE);
	for (i = 0; i < INITIAL_ENVP_SIZE; i++)
		args->aliasList[i] = NULL;
}

/**
 * runShell - loops the prompt
 *
 * @prompt: prompt
 *
 * @args: infinite loop for prompt
 */
void runShell(char *prompt, PROGARGS *args)
{
	int err = 0, len = 0;

	while (++(args->count))
	{
		_print(prompt);
		err = len = _getline(args);

		if (err == EOF)
		{
			freeArgs(args);
			exit(errno);
		}
		if (len >= 1)
		{
			expandAlias(args);
			replaceVariables(args);
			processCommand(args);
			if (args->tokens[0])
			{
				err = executeCommand(args);
				if (err)
					printErr(err, args);
			}
			cleanupAfterExecution(args);
		}
	}
}

/**
 * findExecutable - look for an executable file  in path
 *
 * @args: commands passed
 *
 * Return: 0 success, error code on fail
 */

int findExecutable(PROGARGS *args)
{
	int i = 0, result = 0;
	char **dirs;

	if (!args->cmd)
		return (2);

	if (args->cmd[0] == '/' || args->cmd[0] == '.')
		return (checkFile(args->cmd));

	free(args->tokens[0]);
	args->tokens[0] = _strcat(_strdup("/"), args->cmd);
	if (!args->tokens[0])
		return (2);

	dirs = getPath(args);

	if (!dirs || !dirs[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], args->tokens[0]);
		result = checkFile(dirs[i]);
		if (result == 0 || result == 126)
		{
			errno = 0;
			free(args->tokens[0]);
			args->tokens[0] = _strdup(dirs[i]);
			freePtrs(dirs);
			return (result);
		}
	}
	free(args->tokens[0]);
	args->tokens[0] = NULL;
	freePtrs(dirs);
	return (result);
}
