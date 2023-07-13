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
		{"exit", exitAction},
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
			perror("cd error");
			return;
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		targetDir = getenv("OLDPWD");
		if (!targetDir)
		{
			perror("cd error");
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
		perror("cd error");
		return (-1);
	}

	if (chdir(targetDir))
	{
		perror("cd error");
		return (-1);
	}

	if (setenv("OLDPWD", cwd, 1))
	{
		perror("cd error");
		return (-1);
	}
	return (0);
}

/**
 * exitAction - Exits the shell with the specified exit status.
 *
 * @args: An array of command arguments where args[1]
 * contains the exit status.
 *
 * This function terminates the shell with the specified
 * exit status. If no exit status is provided, the default
 * exit status is 0.
 *
 * Return: Void
 */
void exitAction(char **args)
{
	int status = 0;

	if (args[1])
		status = atoi(args[1]);
	exit(status);
}

