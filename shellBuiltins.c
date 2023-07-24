#include "shell.h"

/**
 * checkBuiltins - checks if a command is a built-in one and
 * performs associated action
 *
 * @cmd: the command to be checked
 *
 * @args: the arguments passed to the command
 *
 * Return: 1 if the command is a built-in one else 0
 */
int checkBuiltins(char *cmd, char **args)
{
	int i;

	BuiltinCmd builtins[] = {
		{"cd", cdAction},
		{NULL, NULL}
	};

	i = 0;

	while (builtins[i].cmd)
	{
		if (_strcmp(cmd, builtins[i].cmd) == 0)
		{
			if (builtins[i].action)
				builtins[i].action(args);
			return (1); /* it's a built-in command */
		}
		i++;
	}
	return (0); /* not a built-in command */
}

/**
 * cdAction - change directory to the specified path.
 *
 * @args: An array of command arguments where args[1]
 * contains the target directory path.
 *
 * This function changes the current working directory
 * to the specified path.
 *
 * If no argument is provided, it displays an error message.
 * If the argument is "~", it changes the directory to the
 * home directory.
 * If the argument is "-", it changes the directory to the
 * previous directory.
 * The function also updates the OLDPWD environment variable
 *and displays the new path.
 *
 * Return: Void
 */
void cdAction(char **args)
{
	char *targetDir;

	if (args[1] == NULL || _strcmp(args[1], "") == 0 ||
			_strcmp(args[1], "~") == 0 || _strcmp(args[1], "$HOME") == 0)
	{
		targetDir = getenv("HOME");
		if (!targetDir)
		{
			perror("./hsh: cd error");
			return;
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		targetDir = getenv("OLDPWD");
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

