#include "shell.h"

/**
 * checkBuiltins - checks if a command is a built-in one and
 * performs associated action
 *
 * @args: The arguments passed to the checked
 *
 * Return: 1 return executed cmd  else -1
 */
int checkBuiltins(PROGARGS *args)
{
	int i;

	BuiltinCmd builtins[] = {
		{"cd", cdAction},
		{"setenv", setenvAction},
		{"unsetenv", unsetenvAction},
		{NULL, NULL}
	};

	i = 0;

	while (builtins[i].command)
	{
		if (_strcmp(builtins[i].command), args->cmd, 0)
		{
			return (builtins[i].action(args));
		}
		i++;
	}
	return (-1);
}

/**
 * cdAction - change directory to the specified path.
 *
 * @args: An array of command arguments
 *
 * Return: 0 on sucess
 */
int cdAction(char **args, char **envp)
{
	char *targetDir;

	if (args[1] == NULL || _strcmp(args[1], "") == 0 ||
			_strcmp(args[1], "~") == 0 || _strcmp(args[1], "$HOME") == 0)
	{
		targetDir = _getenv("HOME", envp);
		if (!targetDir)
		{
			perror("./hsh: cd error");
			return;
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		targetDir = _getenv("OLDPWD", envp);
		if (!targetDir)
		{
			perror("./hsh: cd error");
			return;
		}
		write(STDOUT_FILENO, targetDir, _strlen(targetDir));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
		targetDir = args[1];

	if (changeDirectory(targetDir))
		return;
}

/**
 * changeDirectory - Change the current working directory to the
 * target directory.
 *
 * @targetDir: The target directory to change to.
 *
 * Return: On success, returns 0. On failure, returns -1.
 */
int changeDirectory(char *targetDir)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("./hsh: cd error");
		return (-1);
	}

	if (chdir(targetDir))
	{
		perror("./hsh: cd error");
		return (-1);
	}

	if (setenv("OLDPWD", cwd, 1))
	{
		perror("./hsh: cd error");
		return (-1);
	}
	return (0);
}

/**
 * setenvAction - Initialize a new environment variable or modify an
 * existing one.
 *
 * @args: An array of command arguments where args[1] is the variable name and
 * args[2] is the value.
 *
 * @envp: The environment variables
 *
 * This function sets or modifies the value of an environment variable.
 *
 * Return: Void
 */
void setenvAction(char **args, char **envp)
{
	(void)envp;

	if (!args[1] || !args[2])
	{
		perror("./hsh: setenv: invalid arguments");
		return;
	}

	if (setenv(args[1], args[2], 1))
	{
		perror("./hsh: setenv error");
		return;
	}
}

/**
 * unsetenvAction - Remove an environment variable.
 *
 * @args: An array of command arguments where args[1] is the variable name to
 * be removed.
 *
 * @envp: The environment variables
 *
 * This function removes the specified environment variable.
 *
 * Return: Void
 */
void unsetenvAction(char **args, char **envp)
{
	(void)envp;

	if (!args[1])
	{
		perror("./hsh: unsetenv: missing variable name");
		return;
	}

	if (unsetenv(args[1]))
	{
		perror("./hsh: unsetenv error");
		return;
	}
}
