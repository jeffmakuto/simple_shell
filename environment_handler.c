#include "shell.h"

/**
 * env_action - show the environment
 * @info: commands passed
 * Return: 0 on success, number declared in args
 */
int env_action(info_t *info)
{
	int i;
	char var_name[50] = {'\0'};
	char *var_copy = NULL;

	if (info->tokens[1] == NULL)
		_printenv(info);
	else
	{
		for (i = 0; info->tokens[1][i]; i++)
		{
			if (info->tokens[1][i] == '=')
			{
				var_copy = _strdup(_getenv(var_name, info));
				if (var_copy != NULL)
					_setenv(var_name, info->tokens[1] + i + 1, info);

				_printenv(info);
				if (_getenv(var_name, info) == NULL)
				{
					_print(info->tokens[1]);
					_print("\n");
				}
				else
				{
					_setenv(var_name, var_copy, info);
					free(var_copy);
				}
				return (0);
			}
			var_name[i] = info->tokens[1][i];
		}
		errno = 2;
		perror(info->cmd);
		errno = 127;
	}
	return (0);
}

/**
 * setenv_action - set environment
 * @info: commands passed
 * Return: 0 on sucess, number declared in args
 */
int setenv_action(info_t *info)
{
	if (info->tokens[1] == NULL || info->tokens[2] == NULL)
		return (0);
	if (info->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(info->cmd);
		return (5);
	}

	_setenv(info->tokens[1], info->tokens[2], info);

	return (0);
}

/**
 * unsetenv_action - unset the environment
 * @info: commands passed
 * Return: .void
 */
int unsetenv_action(info_t *info)
{
	if (info->tokens[1] == NULL)
		return (0);
	if (info->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(info->cmd);
		return (5);
	}
	_unsetenv(info->tokens[1], info);

	return (0);
}
