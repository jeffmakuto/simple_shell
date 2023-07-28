#include "shell.h"

/**
 * expandAlias - expand the alias
 *
 * @args: commands passed
 *
 * Return: void
 */
void expandAlias(PROGARGS *args)
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

		temp = getAlias(args, expansion);
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
 * aliasAction - handle aliases
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int aliasAction(PROGARGS *args)
{
	int i = 0;

	if (args->tokens[1] == NULL)
		return (printAlias(args, NULL));

	while (args->tokens[++i])
	{
		if (countChars(args->tokens[i], "="))
			setAlias(args->tokens[i], args);
		else
			printAlias(args, args->tokens[i]);
	}

	return (0);
}


/**
 * printAlias - handle alias
 *
 * @args: commands passed
 *
 * @alias: name of the alias
 *
 * Return: 0 on sucess
 */
int printAlias(PROGARGS *args, char *alias)
{
	int i, j, aliasLen;
	char buffer[BUFFER_SIZE] = {'\0'};

	if (args->aliasList)
	{
		aliasLen = _strlen(alias);
		for (i = 0; args->aliasList[i]; i++)
		{
			if (!alias || (_strncmp(args->aliasList[i], alias, aliasLen)
				&&	args->aliasList[i][aliasLen] == '='))
			{
				for (j = 0; args->aliasList[i][j]; j++)
				{
					buffer[j] = args->aliasList[i][j];
					if (args->aliasList[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buffcat(buffer, "'");
				buffcat(buffer, args->aliasList[i] + j + 1);
				buffcat(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * getAlias -handle alias
 *
 * @args: commands passed
 *
 * @name: name of the requested alias.
 *
 * Return: 0 on success, number declared in the args, NULL if not found
 */
char *getAlias(PROGARGS *args, char *name)
{
	int i, aliasLen;

	if (name == NULL || args->aliasList == NULL)
		return (NULL);

	aliasLen = _strlen(name);

	for (i = 0; args->aliasList[i]; i++)
	{/* check if varname exist in env */
		if (_strncmp(name, args->aliasList[i], aliasLen) &&
			args->aliasList[i][aliasLen] == '=')
		{/* returns the value of the key NAME=  when find it */
			return (args->aliasList[i] + aliasLen + 1);
		}
	}
	return (NULL);

}

/**
 * setAlias - creates alias and overwrites one if it exists
 *
 * @aliasStr: alias to be set
 *
 * @args: commands passed
 *
 * Return: 0 on sucess
 */
int setAlias(char *aliasStr, PROGARGS *args)
{
	int i, j;
	char buffer[BUFFER_SIZE] = {'0'}, *temp = NULL;

	if (aliasStr == NULL ||  args->aliasList == NULL)
		return (1);
	for (i = 0; aliasStr[i]; i++)
		if (aliasStr[i] != '=')
			buffer[i] = aliasStr[i];
		else
		{/* check if value of the alias already exists in another alias */
			temp = getAlias(args, aliasStr + i + 1);
			break;
		}

	/* check if varname exist in the alias list */
	for (j = 0; args->aliasList[j]; j++)
		if (_strncmp(buffer, args->aliasList[j], i) &&
			args->aliasList[j][i] == '=')
		{/* if the alias alredy exist */
			free(args->aliasList[j]);
			break;
		}

	/* add alias */
	if (temp)
	{/* override existing alias */
		buffcat(buffer, "=");
		buffcat(buffer, temp);
		args->aliasList[j] = _strdup(buffer);
	}
	else
		args->aliasList[j] = _strdup(aliasStr);
	return (0);
}
