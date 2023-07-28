#include "shell.h"

/**
 * checkBuiltins - checks if a command is a built-in one and
 * performs associated action
 *
 * @args: commands passed
 *
 * Return: Return of the executed action, or -1.
 */
int checkBuiltins(PROGARGS *args)
{
	int i;

	BuiltinCmd builtins[] = {
		{"cd", cdAction},
		{"exit", exitAction},
		{"setenv", setenvAction},
		{"unsetenv", unsetenvAction},
		{"env", envAction},
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
 * cdAction - change directory to the specified path.
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int cdAction(PROGARGS *args)
{
	char *homeDir = _getenv("HOME", args), *oldDir = NULL;
	char prevDir[MAX_PATH_LEN] = {0};
	int err = 0;

	if (args->tokens[1])
	{
		if (_strncmp(args->tokens[1], "-", 0))
		{
			oldDir = _getenv("OLDPWD", args);
			if (oldDir)
				err = changeDirectory(args, oldDir);
			_print(_getenv("PWD", args));
			_print("\n");

			return (err);
		}
		else
		{
			return (changeDirectory(args, args->tokens[1]));
		}
	}
	else
	{
		if (!homeDir)
			homeDir = getcwd(prevDir, MAX_PATH_LEN);

		return (changeDirectory(args, homeDir));
	}
	return (0);
}

/**
 * changeDirectory - hange the current working directory to the
 * target directory.
 *
 * @args: commands passed
 *
 * @newDir: path to be set as current work directory
 *
 * Return: 0 on success, number declared in args
 */
int changeDirectory(PROGARGS *args, char *newDir)
{
	char prevDir[MAX_PATH_LEN] = {0};
	int err = 0;

	getcwd(prevDir, 128);

	if (!_strncmp(prevDir, newDir, 0))
	{
		err = chdir(newDir);
		if (err == -1)
		{
			errno = 2;
			return (3);
		}
		_setenv("PWD", newDir, args);
	}
	_setenv("OLDPWD", prevDir, args);
	return (0);
}

/**
 * setenvAction - initialize a new environment variable or modify an
 * existing one.
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int setenvAction(PROGARGS *args)
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
 * _unsetenv - remove an environment variable.
 *
 * @args: commands passed
 *
 * Return: .void
 */
int unsetenvAction(PROGARGS *args)
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
