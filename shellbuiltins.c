#include "shell.h"

/**
 * check_builtins - checks if a command is a built-in one and
 * performs associated action
 *
 * @cmd: the command to be checked
 *
 * @args: the arguments passed to the command
 *

 * @envp: The environment variables
 *
 * Return: 1 if the command is a built-in one else 0
 */int check_builtins(char *cmd, char **args, char **envp)
{
	int i;

	builtin_cmd builtins[] = {
		{"cd", cd_action},
		{"setenv", setenv_action},
		{"unsetenv", unsetenv_action},
		{NULL, NULL}
	};

	i = 0;

	while (builtins[i].cmd)
	{
		if (_strcmp(cmd, builtins[i].cmd) == 0)
		{
			if (builtins[i].action)
				builtins[i].action(args, envp);
			return (1); /* it's a built-in command */
		}
		i++;
	}
	return (0); /* not a built-in command */
}

/**
 * cd_action - change directory to the specified path.
 *
 * @args: An array of command arguments where args[1]
 * contains the target directory path.
 *
 * @envp: The environment variables
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
void cd_action(char **args, char **envp)
{
	char *target_dir;

	if (args[1] == NULL || _strcmp(args[1], "") == 0 ||
			_strcmp(args[1], "~") == 0 || _strcmp(args[1], "$HOME") == 0)
	{
		target_dir = _getenv("HOME", envp);
		if (!target_dir)
		{
			perror("./hsh: cd error");
			return;
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		target_dir = _getenv("OLDPWD", envp);
		if (!target_dir)
		{
			perror("./hsh: cd error");
			return;
		}
		write(STDOUT_FILENO, target_dir, _strlen(target_dir));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
		target_dir = args[1];

	if (change_directory(target_dir))
		return;
}

/**
 * change_directory - Change the current working directory to the
 * target directory.
 *
 * @target_dir: The target directory to change to.
 *
 * Return: On success, returns 0. On failure, returns -1.
 */
int change_directory(char *target_dir)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("./hsh: cd error");
		return (-1);
	}

	if (chdir(target_dir))
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
 * setenv_action - Initialize a new environment variable or modify an
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
void setenv_action(char **args, char **envp)
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
 * unsetenv_action - Remove an environment variable.
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
void unsetenv_action(char **args, char **envp)
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

