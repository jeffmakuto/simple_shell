#include "shell.h"

/**
 * _getenv -  retrieves the value of the environment variable specified by
 * the 'name' argument.
 *
 * @name: the name of the environment variable to retrieve.
 *
 * @args: commands passed
 *
 * Return: pointer to value of var or NULL
 */
char *_getenv(char *name, PROGARGS *args)
{
	int i, nameLen = 0;

	if (name == NULL || args->envp == NULL)
		return (NULL);

	nameLen = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], nameLen) &&
				args->envp[i][nameLen] == '=')
		{
			return (args->envp[i] + nameLen + 1);
		}
	}
	return (NULL);
}

/**
 * _setenv - overwrites environment var or creates one
 *
 * @name: name of the environment variable to set
 *
 * @value: new value
 *
 * @args: commands passed
 *
 * Return: 1 if pars are NULL, 2 on error, 0 on sucess.
 */

int _setenv(char *name, char *value, PROGARGS *args)
{
	int i, nameLen = 0, newVar = 1;

	if (name == NULL || value == NULL || args->envp == NULL)
		return (1);

	nameLen = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], nameLen) &&
				args->envp[i][nameLen] == '=')
		{
			newVar = 0;

			free(args->envp[i]);
			break;
		}
	}
	args->envp[i] = _strcat(_strdup(name), "=");
	args->envp[i] = _strcat(args->envp[i], value);

	if (newVar)
	{
		args->envp[i + 1] = NULL;
	}
	return (0);
}

/**
 * _unsetenv - deletes the created var from the environment
 *
 * @name: the variable to delete
 *
 * @args: commands passed
 *
 * Return: 1 if the var removed, 0 if the var does not exist;
 */
int _unsetenv(char *name, PROGARGS *args)
{
	int i, nameLen = 0;

	if (key == NULL || args->envp == NULL)
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
 * _printenv - print current environment
 *
 * @args: commands passed
 *
 * Return: void
 */
void _printenv(PROGARGS *args)
{
	int j;

	for (j = 0; args->envp[j]; j++)
	{
		_print(args->envp[j]);
		_print("\n");
	}
}

/**
 * envAction - show thevenvironment
 *
 * @args: commands
 *
 * Return: 0 on success
 */
int envAction(PROGARGS)
{
	int i;
	char varName[MAX_PATH_LEN] = {'\0'};
	char *varCopy = NULL;

	if (args->tokens[1] == NULL)
		_printenv(args);
	else
	{
		for (i = 0; args->tokens[1][i]; i++)
		{
			if (args->tokens[1][i] == '=')
			{
				varCopy = _strdup(_getenv(varName, args));
				if (varCopy)
					_setenv(varName, args->tokens[1] + i + 1, args);

				_printenv(args);
				if (_getenv(varName, args) == NULL)
				{
					_print(args->tokens[1]);
					_print("\n");
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
		errno = 2;
		perror(args->cmd);
		errno = 127;
	}
	return (0);
}
