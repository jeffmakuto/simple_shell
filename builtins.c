#include "shell.h"

/**
 * exit_action - exit the shell
 * @info: commands passed
 * Return: 0 on sucess,number declared in the args
 */
int exit_action(info_t *info)
{
	int i;

	if (info->tokens[1] != NULL)
	{
		for (i = 0; info->tokens[1][i]; i++)
			if ((info->tokens[1][i] < '0' || info->tokens[1][i] > '9')
				&& info->tokens[1][i] != '+')
			{
				errno = 2;
				return (2);
			}
		errno = _atoi(info->tokens[1]);
	}
	free_info(info);
	exit(errno);
}

/**
 * cd_action - change directory
 * @info: commands passed
 * Return: 0 on sucess, number declared in the args
 */
int cd_action(info_t *info)
{
	char *home_dir = _getenv("HOME", info), *old_dir = NULL;
	char prev_dir[128] = {0};
	int err = 0;

	if (info->tokens[1])
	{
		if (_strncmp(info->tokens[1], "-", 0))
		{
			old_dir = _getenv("OLDPWD", info);
			if (old_dir)
				err = change_dir(info, old_dir);
			_print(_getenv("PWD", info));
			_print("\n");

			return (err);
		}
		else
		{
			return (change_dir(info, info->tokens[1]));
		}
	}
	else
	{
		if (!home_dir)
			home_dir = getcwd(prev_dir, 128);

		return (change_dir(info, home_dir));
	}
	return (0);
}

/**
 * set_work_directory - set work dir
 * @info: commands passed
 * @new_dir: path to be set as work dir
 * Return: 0 on success, number declared in args
 */
int change_dir(info_t *info, char *new_dir)
{
	char prev_dir[128] = {0};
	int err = 0;

	getcwd(prev_dir, 128);

	if (!_strncmp(prev_dir, new_dir, 0))
	{
		err = chdir(new_dir);
		if (err == -1)
		{
			errno = 2;
			return (3);
		}
		_setenv("PWD", new_dir, info);
	}
	_setenv("OLDPWD", prev_dir, info);
	return (0);
}

/**
 * help_action - env of shell
 * @info: commands passed
 * Return: 0 on sucess, number declared in args
 */
int help_action(info_t *info)
{
	int i, len = 0;
	char *help[6] = {NULL};

	help[0] = HELP_MSG;

	if (info->tokens[1] == NULL)
	{
		_print(help[0] + 6);
		return (1);
	}
	if (info->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(info->cmd);
		return (5);
	}
	help[1] = HELP_EXIT_MSG;
	help[2] = HELP_ENV_MSG;
	help[3] = HELP_SETENV_MSG;
	help[4] = HELP_UNSETENV_MSG;
	help[5] = HELP_CD_MSG;

	for (i = 0; help[i]; i++)
	{
		len = _strlen(info->tokens[1]);
		if (_strncmp(info->tokens[1], help[i], len))
		{
			_print(help[i] + len + 1);
			return (1);
		}
	}
	errno = EINVAL;
	perror(info->cmd);
	return (0);
}

/**
 * alias_action - handle aliases
 * @info: commands passed
 * Return: 0 on sucess, number declared in args
 */
int alias_action(info_t *info)
{
	int i = 0;

	if (info->tokens[1] == NULL)
		return (print_alias(info, NULL));

	while (info->tokens[++i])
	{
		if (count_chars(info->tokens[i], "="))
			set_alias(info->tokens[i], info);
		else
			print_alias(info, info->tokens[i]);
	}

	return (0);
}
