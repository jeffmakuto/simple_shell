#include "shell.h"
/**
 * main - start the shell
 * @ac: argument count
 * @av: argument vector
 * @env:no of values from the command line
 * Return: 0 on success
 */
int main(int ac, char *av[], char *env[])
{
	info_t info_struct = {NULL}, *info = &info_struct;
	char *prompt = NULL;

	start_shell(info, ac, av, env);

	signal(SIGINT, handle_ctrl_c);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && ac == 1)
	{
		errno = 2;
		prompt = PROMPT_MSG;
	}
	errno = 0;
	run_shell(prompt, info);
	return (0);
}

/**
 * handle_ctrl_c - print the prompt
 * @UNUSED: option of the prototype
 */
void handle_ctrl_c(int signal)
{
	(void)signal;
	_print("\n");
	_print(PROMPT_MSG);
}

/**
 * start_shell - initialize struct with the info of the shell
 * @info: commands passed
 * @ac: argument count
 * @av: argument vector
 * @env: environment variables
 */
void start_shell(info_t *info, int ac, char *av[], char **env)
{
	int i = 0;

	info->prog_name = av[0];
	info->buff = NULL;
	info->cmd = NULL;
	info->count = 0;

	if (ac == 1)
		info->fd = STDIN_FILENO;
	else
	{
		info->fd = open(av[1], O_RDONLY);
		if (info->fd == -1)
		{
			_printe(info->prog_name);
			_printe(": 0: Can't open ");
			_printe(av[1]);
			_printe("\n");
			exit(127);
		}
	}
	info->tokens = NULL;
	info->env = malloc(sizeof(char *) * 50);
	if (env)
	{
		for (; env[i]; i++)
		{
			info->env[i] = _strdup(env[i]);
		}
	}
	info->env[i] = NULL;
	env = info->env;

}
/**
 * run_shell - loops the prompt
 * @prompt: prompt
 * @info: commands passed
 */
void run_shell(char *prompt, info_t *info)
{
	int err = 0, len = 0;

	while (++(info->count))
	{
		_print(prompt);
		err = len = _getline(info);

		if (err == EOF)
		{
			free_info(info);
			exit(errno);
		}
		if (len >= 1)
		{
			expand_variables(info);
			process_command(info);
			if (info->tokens[0])
			{
				err = execute(info);
				if (err != 0)
					_print_error(err, info);
			}
			cleanup_after_execution(info);
		}
	}
}
