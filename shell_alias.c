#include "shell.h"

/**
 * expand_alias - expand the alias
 *
 * @args: commands passed
 *
 * Return: void
 */
void expand_alias(prog_args *args)
{
	int i, j, expanded = 0;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (args->buffer == NULL)
		return;

	buffcat(line, args->buffer);

	for (i = 0; line[i]; i++)
	{
		for (j = 0; line[i + j] && line[i + j] != ' '; j++)
			expansion[j] = line[i + j];
		expansion[j] = '\0';

		temp = get_alias(args, expansion);
		if (temp)
		{
			expansion[0] = '\0';
			buffcat(expansion, line + i + j);
			line[i] = '\0';
			buffcat(line, temp);
			line[_strlen(line)] = '\0';
			buffcat(line, expansion);
			expanded = 1;
		}
		break;
	}
	if (expanded)
	{
		free(args->buffer);
		args->buffer = _strdup(line);
	}
}

/**
 * alias_action - handle aliases
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int alias_action(prog_args *args)
{
	int i = 0;

	if (args->tokens[1] == NULL)
		return (print_alias(args, NULL));

	while (args->tokens[++i])
	{
		if (count_chars(args->tokens[i], "="))
			set_alias(args->tokens[i], args);
		else
			print_alias(args, args->tokens[i]);
	}

	return (0);
}


/**
 * print_alias - handle alias
 *
 * @args: commands passed
 *
 * @alias: name of the alias
 *
 * Return: 0 on sucess
 */
int print_alias(prog_args *args, char *alias)
{
	int i, j, alias_len;
	char buffer[BUFFER_SIZE] = {'\0'};

	if (args->alias_list)
	{
		alias_len = _strlen(alias);
		for (i = 0; args->alias_list[i]; i++)
		{
			if (!alias || (_strncmp(args->alias_list[i], alias, alias_len)
				&&	args->alias_list[i][alias_len] == '='))
			{
				for (j = 0; args->alias_list[i][j]; j++)
				{
					buffer[j] = args->alias_list[i][j];
					if (args->alias_list[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buffcat(buffer, "'");
				buffcat(buffer, args->alias_list[i] + j + 1);
				buffcat(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * get_alias -handle alias
 *
 * @args: commands passed
 *
 * @name: name of the requested alias.
 *
 * Return: 0 on success, number declared in the args, NULL if not found
 */
char *get_alias(prog_args *args, char *name)
{
	int i, alias_len;

	if (name == NULL || args->alias_list == NULL)
		return (NULL);

	alias_len = _strlen(name);

	for (i = 0; args->alias_list[i]; i++)
	{/* check if varname exist in env */
		if (_strncmp(name, args->alias_list[i], alias_len) &&
			args->alias_list[i][alias_len] == '=')
		{/* returns the value of the key NAME=  when find it */
			return (args->alias_list[i] + alias_len + 1);
		}
	}
	return (NULL);

}

/**
 * set_alias - creates alias and overwrites one if it exists
 *
 * @alias_str: alias to be set
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int set_alias(char *alias_str, prog_args *args)
{
	int i, j;
	char buffer[BUFFER_SIZE] = {'0'}, *temp = NULL;

	if (alias_str == NULL ||  args->alias_list == NULL)
		return (1);
	for (i = 0; alias_str[i]; i++)
		if (alias_str[i] != '=')
			buffer[i] = alias_str[i];
		else
		{/* check if value of the alias already exists in another alias */
			temp = get_alias(args, alias_str + i + 1);
			break;
		}

	/* check if varname exist in the alias list */
	for (j = 0; args->alias_list[j]; j++)
		if (_strncmp(buffer, args->alias_list[j], i) &&
			args->alias_list[j][i] == '=')
		{/* if the alias alredy exist */
			free(args->alias_list[j]);
			break;
		}

	/* add alias */
	if (temp)
	{/* override existing alias */
		buffcat(buffer, "=");
		buffcat(buffer, temp);
		args->alias_list[j] = _strdup(buffer);
	}
	else
		args->alias_list[j] = _strdup(alias_str);
	return (0);
}
