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
		{"env", _envp},
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
 * cdAction - Change directory to the specified path.
 *
 * @args: An array of command arguments
 *
 * Return: 0 on sucess
 */
int cdAction(PROGARGS *args)
{
	char *homeDir = _getenv("HOME", args), *oldDir = NULL;
	char prevDir[MAX_PATH_LEN] = {0};
	int result = 0;

	if (args->tokens[1])
	{
		if (_strcomp(args->tokens[1], "-", 0))
		{
			oldDir = _getenv("OLDPWD", args);
			if (oldDir)
				result = changeDirectory(args, oldDir);
			write(STDOUT_FILENO, _getenv("PWD", args), _strlen(_getenv("PWD", args)));
			write(STDOUT_FILENO, "\n", 1);
			return (result);
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
 * changeDirectory - Set working directory
 *
 * @args: An array of arguments
 *
 * @newDir: Path to be set as working directory
 *
 * Return: 0 on success, number declared in args
 */
int changeDirectory(PROGARGS *args, char *newDir)
{
	char prevDir[MAX_PATH_LEN] = {0};
	int result = 0;

	getcwd(prevDir, MAX_PATH_LEN);

	if (!_strcomp(prevDir, newDir, 0))
	{
		result = chdir(newDir);
		if (result == -1)
		{
			perror("./hsh: chdir error");
			return (1);
		}
		setenvAction("PWD", newDir, args);
	}
	setenvAction("OLDPWD", prevDir, args);
	return (0);
}

/**
 * _envp - show the environment
 *
 * @args: The arguments passed
 *
 * Return: 0 on success
 */
int _envp(PROGARGS *args)
{
	int i;
	char varName[MAX_PATH_LEN] = {'\0'};
	char *varCopy = NULL;

	if (args->tokens[1] == NULL)
		printEnv(args);
	else
	{
		for (i = 0; args->tokens[1][i]; i++)
		{
			if (args->tokens[1][i] == '=')
			{
				varCopy = _strdup(_getenv(varName, args));
				if (varCopy != NULL)
					_setenv(varName, args->tokens[1] + i + 1, args);

				print_env(args);
				if (_getenv(varName, args) == NULL)
				{
					write(STDOUT_FILENO, args->tokens[1], _strlen(args->tokens[1]));
					write(STDOUT_FILENO, "\n", 1);
				}
				else
				{
					_setenv(varName, varCopy, args);
					free(varCopy);
				}
				return (0);
			}
			varName[i] = args->tokens[1][i];
		}
		perror("./hsh:");
	}
	return (0);
}
