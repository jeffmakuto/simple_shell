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
		{"setenv", setenvAction},
		{"unsetenv", unsetenvAction},
		{NULL, NULL}
	};

	i = 0;

	while (builtins[i].cmd)
	{
		if (strcmp(cmd, builtins[i].cmd) == 0)
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

	if (args[1] == NULL || strcmp(args[1], "") == 0 ||
			strcmp(args[1], "~") == 0 || strcmp(args[1], "$HOME") == 0)
	{
		targetDir = getenv("HOME");
		if (!targetDir)
		{
			perror("./hsh: cd error");
			return;
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		targetDir = getenv("OLDPWD");
		if (!targetDir)
		{
			perror("./hsh: cd error");
			return;
		}
		write(STDOUT_FILENO, targetDir, strlen(targetDir));
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
int changeDirectory(const char *targetDir)
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
 * setenvAction - Set environment variable with name and value.
 *
 * @args: An array of command arguments. args[1] should be the variable name,
 * and args[2] should be the value to set.
 *
 * This function sets an environment variable with the given name and value.
 * If either the variable name or value is missing, it prints an error message.
 * If setting the environment variable fails, it prints an error using perror.
 *
 * Return: Void
 */
void setenvAction(char **args)
{
	if (!args[1] || !args[2])
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return;
	}
	if (setenv(args[1], args[2], 1))
		perror("setenv");
}

/**
 * unsetenvAction - Unset environment variable.
 *
 * @args: An array of command arguments. args[1] should be
 * the variable name to unset.
 *
 * This function unsets an environment variable with the given name.
 * If the variable name is missing, it prints an error message.
 * If unsetting the environment variable fails, it prints an error
 * using perror.
 *
 * Return: Void
 */
void unsetenvAction(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return;
	}
	if (unsetenv(args[1]) != 0)
		perror("unsetenv");
}

