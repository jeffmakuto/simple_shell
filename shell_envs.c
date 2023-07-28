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
char *_getenv(char *name, prog_args *args)
{
	int i, name_len = 0;

	if (name == NULL || args->envp == NULL)
		return (NULL);

	name_len = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], name_len) &&
				args->envp[i][name_len] == '=')
		{
			return (args->envp[i] + name_len + 1);
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

int _setenv(char *name, char *value, prog_args *args)
{
	int i, name_len = 0, new_var = 1;

	if (name == NULL || value == NULL || args->envp == NULL)
		return (1);

	name_len = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], name_len) &&
				args->envp[i][name_len] == '=')
		{
			new_var = 0;

			free(args->envp[i]);
			break;
		}
	}
	args->envp[i] = _strcat(_strdup(name), "=");
	args->envp[i] = _strcat(args->envp[i], value);

	if (new_var)
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
int _unsetenv(char *name, prog_args *args)
{
	int i, name_len = 0;

	if (name == NULL || args->envp == NULL)
		return (0);

	name_len = _strlen(name);

	for (i = 0; args->envp[i]; i++)
	{
		if (_strncmp(name, args->envp[i], name_len) &&
				args->envp[i][name_len] == '=')
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
void _printenv(prog_args *args)
{
	int j;

	for (j = 0; args->envp[j]; j++)
	{
		_print(args->envp[j]);
		_print("\n");
	}
}

/**
 * env_action - show thevenvironment
 *
 * @args: commands
 *
 * Return: 0 on success
 */
int env_action(prog_args *args)
{
	int i;
	char var_name[MAX_PATH_LEN] = {'\0'};
	char *var_copy = NULL;

	if (args->tokens[1] == NULL)
		_printenv(args);
	else
	{
		for (i = 0; args->tokens[1][i]; i++)
		{
			if (args->tokens[1][i] == '=')
			{
				var_copy = _strdup(_getenv(var_name, args));
				if (var_copy)
					_setenv(var_name, args->tokens[1] + i + 1, args);

				_printenv(args);
				if (_getenv(var_name, args) == NULL)
				{
					_print(args->tokens[1]);
					_print("\n");
				}
				else
				{
					_setenv(var_name, var_copy, args);
					free(var_copy);
				}
				return (0);
			}
			var_name[i] = args->tokens[1][i];
		}
		errno = 2;
		perror(args->cmd);
		errno = 127;
	}
	return (0);
}
