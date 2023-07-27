#include "shell.h"

/**
 * _setenv - Overwrite an env var or create one
 *
 * @name: name of the var
 *
 * @value: new value
 *
 * @args: Arguments passed
 *
 * Return: 1 if pars are NULL, 2 on error, 0 on sucess.
 */

int _setenv(char *name, char *value, PROGARGS *args)
{
	int i, nameLen = 0, newKey = 1;

	if (name == NULL || value == NULL || args->envp == NULL)
		return (1);

	nameLen = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], nameLen) &&
		 args->envp[i][nameLen] == '=')
		{
			newKey = 0;

			free(args->envp[i]);
			break;
		}
	}
	args->envp[i] = _strcat(_strdup(name), "=");
	args->envp[i] = _strcat(args->envp[i], value);

	if (newKey)
	{
		args->envp[i + 1] = NULL;
	}
	return (0);
}

/**
 * _unsetenv - Remove the environment
 *
 * @name: The environment  to remove
 *
 * @args: Arguments passed
 *
 * Return: 1 if the env removed, 0 if the env  does not exist;
 */
int _unsetenv(char *name, PROGARGS *args)
{
	int i, nameLen = 0;

	if (name == NULL || args->envp == NULL)
		return (0);

	nameLen = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], nameLen) &&
		 args->envp[i][nameLen] == '=')
		{
			free(args->envp[i]);

			i++;
			for (; args->envp[i]; i++)
			{
				args->envp[i - 1] = args->envp[i];
			}
			args->envp[i - 1] = NULL;
			return (1);
		}
	}
	return (0);
}


/**
 * _printenv - Print current environment
 *
 * @args: Arguments passed
 *
 * Return: Void
 */
void _printenv(PROGARGS *args)
{
	int j;

	for (j = 0; args->envp[j]; j++)
	{
		write(STDOUT_FILENO, args->envp[j], _strlen(args->envp[j]));
		write(STDOUT_FILENO, "\n", 1);
	}
}

/**
 * setenvAction - Sets environment
 *
 * @args: Arguments passed
 *
 * Return: 0 on sucess
 */
int setenvAction(PROGARGS *args)
{
	if (args->tokens[1] == NULL || args->tokens[2] == NULL)
		return (0);
	if (args->tokens[3] != NULL)
	{
		perror("./hsh: Error: Too many arguments");
		return (1);
	}

	_setenv(args->tokens[1], args->tokens[2], args);

	return (0);
}

/**
 * unsetenvAction - Unset environment
 *
 * @args: Arguments passed
 *
 * Return: 0 on success
 */
int unsetenvAction(PROGARGS *args)
{
	if (args->tokens[1] == NULL)
		return (0);
	if (args->tokens[2] != NULL)
	{
		perror("./hsh: Error: Too many arguments");
		return (1);
	}
	_unsetenv(args->tokens[1], args);

	return (0);
}
