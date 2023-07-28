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
	char prev_dir[MAX_PATH_LEN] = {0};
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
			home_dir = getcwd(prev_dir, MAX_PATH_LEN);

		return (change_dir(info, home_dir));
	}
	return (0);
}

/**
 * change_dir - set work dir
 * @info: commands passed
 * @new_dir: path to be set as work dir
 * Return: 0 on success, number declared in args
 */
int change_dir(info_t *info, char *new_dir)
{
	char prev_dir[MAX_PATH_LEN] = {0};
	int err = 0;

	getcwd(prev_dir, MAX_PATH_LEN);

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
