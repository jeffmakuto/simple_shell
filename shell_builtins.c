#include "shell.h"

/**
 * check_builtins - checks if a command is a built-in one and
 * performs associated action
 *
 * @args: commands passed
 *
 * Return: Return of the executed action, or -1.
 */
int check_builtins(prog_args *args)
{
	int i;

	builtin_cmd builtins[] = {
		{"cd", cd_action},
		{"exit", exit_action},
		{"setenv", setenv_action},
		{"unsetenv", unsetenv_action},
		{"env", env_action},
		{"alias", alias_action},
		{NULL, NULL}
	};

	for (i = 0; builtins[i].command != NULL; i++)
	{
		if (_strncmp(builtins[i].command, args->cmd, 0))
		{
			return (builtins[i].action(args));
		}
	}
	return (-1);
}

/**
 * cd_action - change directory to the specified path.
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int cd_action(prog_args *args)
{
	char *home_dir = _getenv("HOME", args), *old_dir = NULL;
	char prev_dir[MAX_PATH_LEN] = {0};
	int err = 0;

	if (args->tokens[1])
	{
		if (_strncmp(args->tokens[1], "-", 0))
		{
			old_dir = _getenv("OLDPWD", args);
			if (old_dir)
				err = change_directory(args, old_dir);
			_print(_getenv("PWD", args));
			_print("\n");

			return (err);
		}
		else
		{
			return (change_directory(args, args->tokens[1]));
		}
	}
	else
	{
		if (!home_dir)
			home_dir = getcwd(prev_dir, MAX_PATH_LEN);

		return (change_directory(args, home_dir));
	}
	return (0);
}

/**
 * change_directory - hange the current working directory to the
 * target directory.
 *
 * @args: commands passed
 *
 * @new_dir: path to be set as current work directory
 *
 * Return: 0 on success, number declared in args
 */
int change_directory(prog_args *args, char *new_dir)
{
	char prev_dir[MAX_PATH_LEN] = {0};
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
		_setenv("PWD", new_dir, args);
	}
	_setenv("OLDPWD", prev_dir, args);
	return (0);
}

/**
 * setenv_action - initialize a new environment variable or modify an
 * existing one.
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int setenv_action(prog_args *args)
{
	if (args->tokens[1] == NULL || args->tokens[2] == NULL)
		return (0);
	if (args->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(args->cmd);
		return (5);
	}

	_setenv(args->tokens[1], args->tokens[2], args);

	return (0);
}

/**
 * unsetenv_action - remove an environment variable.
 *
 * @args: commands passed
 *
 * Return: .void
 */
int unsetenv_action(prog_args *args)
{
	if (args->tokens[1] == NULL)
		return (0);
	if (args->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(args->cmd);
		return (5);
	}
	_unsetenv(args->tokens[1], args);

	return (0);
}
