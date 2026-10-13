#include "shell.h"

/**
 * _getenv - get value of environment  variable
 * @name: environment variable
 * @info: commands passed
 * Return: ptr to value of var or NULL
 */
char *_getenv(char *name, info_t *info)
{
	int i, name_len = 0;

	if (name == NULL || info->env == NULL)
		return (NULL);

	name_len = _strlen(name);

	for (i = 0; info->env[i]; i++)
	{
		if (_strncmp(name, info->env[i], name_len) &&
		 info->env[i][name_len] == '=')
		{
			return (info->env[i] + name_len + 1);
		}
	}
	return (NULL);
}

/**
 * _setenv - overwrite environment var or create one
 * @name: name of the var
 * @value: new value
 * @info: commands passed
 * Return: 1 if pars are NULL, 2 on error, 0 on sucess.
 */

int _setenv(char *name, char *value, info_t *info)
{
	int i, j, name_len = 0;
	char *new_var, **new_env;

	if (name == NULL || value == NULL || info->env == NULL)
		return (1);

	name_len = _strlen(name);
	new_var = _strcat(_strcat(_strdup(name), "="), value);
	if (new_var == NULL)
		return (2);

	for (i = 0; info->env[i]; i++)
	{
		if (_strncmp(name, info->env[i], name_len) &&
		 info->env[i][name_len] == '=')
		{
			free(info->env[i]);
			info->env[i] = new_var;
			return (0);
		}
	}

	/* new variable: make room for one more pointer */
	new_env = malloc(sizeof(char *) * (i + 2));
	if (new_env == NULL)
	{
		free(new_var);
		return (2);
	}
	for (j = 0; j < i; j++)
		new_env[j] = info->env[j];
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(info->env);
	info->env = new_env;
	return (0);
}

/**
 * _unsetenv - remove var from env
 * @name: the var to remove
 * @info: commands passed
 * Return: 1 if the key removed, 0 if the key does not exist;
 */
int _unsetenv(char *name, info_t *info)
{
	int i, name_len = 0;

	if (name == NULL || info->env == NULL)
		return (0);

	name_len = _strlen(name);

	for (i = 0; info->env[i]; i++)
	{
		if (_strncmp(name, info->env[i], name_len) &&
		 info->env[i][name_len] == '=')
		{
			free(info->env[i]);

			i++;
			for (; info->env[i]; i++)
			{
				info->env[i - 1] = info->env[i];
			}
			info->env[i - 1] = NULL;
			return (1);
		}
	}
	return (0);
}


/**
 * _printenv - print current env
 * @info: commands passed
 * Return: void
 */
void _printenv(info_t *info)
{
	int j;

	for (j = 0; info->env[j]; j++)
	{
		_print(info->env[j]);
		_print("\n");
	}
}
