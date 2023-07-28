#include "shell.h"

/**
 * print_alias - handle alias
 * @info: commands passed
 * @alias: name of the alias
 * Return: 0 on sucess
 */
int print_alias(info_t *info, char *alias)
{
	int i, j, alias_len;
	char buffer[250] = {'\0'};

	if (info->alias_list)
	{
		alias_len = _strlen(alias);
		for (i = 0; info->alias_list[i]; i++)
		{
			if (!alias || (_strncmp(info->alias_list[i], alias, alias_len)
				&&	info->alias_list[i][alias_len] == '='))
			{
				for (j = 0; info->alias_list[i][j]; j++)
				{
					buffer[j] = info->alias_list[i][j];
					if (info->alias_list[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buffer_cat(buffer, "'");
				buffer_cat(buffer, info->alias_list[i] + j + 1);
				buffer_cat(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * get_alias -handle alias
 * @info: commands passed
 * @name: name of the requested alias.
 * Return: 0 on success, number declared in the args, NULL if not found
 */
char *get_alias(info_t *info, char *name)
{
	int i, alias_len;

	if (name == NULL || info->alias_list == NULL)
		return (NULL);

	alias_len = _strlen(name);

	for (i = 0; info->alias_list[i]; i++)
	{/* check if varname exist in env */
		if (_strncmp(name, info->alias_list[i], alias_len) &&
			info->alias_list[i][alias_len] == '=')
		{/* returns the value of the key NAME=  when find it */
			return (info->alias_list[i] + alias_len + 1);
		}
	}
	return (NULL);

}

/**
 * set_alias - creates alias, overides if exists
 * @alias_str: alias to be set
 * @info: commands passed
 * Return: 0 on sucess, number declared in args
 */
int set_alias(char *alias_str, info_t *info)
{
	int i, j;
	char buffer[250] = {'0'}, *temp = NULL;

	if (alias_str == NULL ||  info->alias_list == NULL)
		return (1);
	for (i = 0; alias_str[i]; i++)
		if (alias_str[i] != '=')
			buffer[i] = alias_str[i];
		else
		{/* check if value of the alias already exists in another alias */
			temp = get_alias(info, alias_str + i + 1);
			break;
		}

	/* check if varname exist in the alias list */
	for (j = 0; info->alias_list[j]; j++)
		if (_strncmp(buffer, info->alias_list[j], i) &&
			info->alias_list[j][i] == '=')
		{/* if the alias alredy exist */
			free(info->alias_list[j]);
			break;
		}

	/* add alias */
	if (temp)
	{/* override existing alias */
		buffer_cat(buffer, "=");
		buffer_cat(buffer, temp);
		info->alias_list[j] = _strdup(buffer);
	}
	else
		info->alias_list[j] = _strdup(alias_str);
	return (0);
}
