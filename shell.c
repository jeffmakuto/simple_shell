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
	prog_args shell_args = {NULL}, *args = &shell_args;
	char *prompt = NULL;

	start_shell(args, ac, av, envp);

	signal(SIGINT, handle_ctrl_c_signal);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && ac == 1)
	{
		errno = 2;
		prompt = PROMPT;
	}
	errno = 0;

	run_shell(prompt, args);

	return (0);
}

/**
 * handle_c_ctrl_c_signal - Handles Cntrl + C
 *
 * @signal: unused parameter.(Signal)
 *
 * Return: void
 */
void handle_ctrl_c_signal(int signal)
{
	(void)signal;
	_print("\n");
	_print(PROMPT);
}

/**
 * start_shell - launches the shell
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
void start_shell(prog_args *args, int ac, char *av[], char **envp)
{
	int i = 0;

	args->prog_name = av[0];
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
			_printe(args->prog_name);
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

	args->alias_list = malloc(sizeof(char *) * INITIAL_ENVP_SIZE);
	for (i = 0; i < INITIAL_ENVP_SIZE; i++)
		args->alias_list[i] = NULL;
}

/**
 * run_shell - loops the prompt
 *
 * @prompt: prompt
 *
 * @args: infinite loop for prompt
 */
void run_shell(char *prompt, prog_args *args)
{
	int err = 0, len = 0;

	while (++(args->count))
	{
		_print(prompt);
		err = len = _getline(args);

		if (err == EOF)
		{
			free_args(args);
			exit(errno);
		}
		if (len >= 1)
		{
			expand_alias(args);
			replace_variables(args);
			process_command(args);
			if (args->tokens[0])
			{
				err = execute_command(args);
				if (err)
					print_err(err, args);
			}
			cleanup_after_execution(args);
		}
	}
}

/**
 * find_executable - look for an executable file  in path
 *
 * @args: commands passed
 *
 * Return: 0 success, error code on fail
 */

int find_executable(prog_args *args)
{
	int i = 0, result = 0;
	char **dirs;

	if (!args->cmd)
		return (2);

	if (args->cmd[0] == '/' || args->cmd[0] == '.')
		return (check_file(args->cmd));

	free(args->tokens[0]);
	args->tokens[0] = _strcat(_strdup("/"), args->cmd);
	if (!args->tokens[0])
		return (2);

	dirs = get_path(args);

	if (!dirs || !dirs[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], args->tokens[0]);
		result = check_file(dirs[i]);
		if (result == 0 || result == 126)
		{
			errno = 0;
			free(args->tokens[0]);
			args->tokens[0] = _strdup(dirs[i]);
			free_ptrs(dirs);
			return (result);
		}
	}
	free(args->tokens[0]);
	args->tokens[0] = NULL;
	free_ptrs(dirs);
	return (result);
}
