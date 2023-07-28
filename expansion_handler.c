#include "shell.h"

/**
 * expand_variables - expand the variable
 * @info: commands passed
 *
 * Return: void
 */
void expand_variables(info_t *info)
{
	int i, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (info->buff == NULL)
		return;
	buffer_cat(line, info->buff);
	for (i = 0; line[i]; i++)
		if (line[i] == '#')
			line[i--] = '\0';
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			long_to_string(errno, expansion, 10);
			buffer_cat(line, expansion);
			buffer_cat(line, info->buff + i + 2);
		}
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			long_to_string(getpid(), expansion, 10);
			buffer_cat(line, expansion);
			buffer_cat(line, info->buff + i + 2);
		}
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		else if (line[i] == '$')
		{
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			temp = _getenv(expansion, info);
			line[i] = '\0', expansion[0] = '\0';
			buffer_cat(expansion, line + i + j);
			temp ? buffer_cat(line, temp) : 1;
			buffer_cat(line, expansion);
		}
	if (!_strncmp(info->buff, line, 0))
	{
		free(info->buff);
		info->buff = _strdup(line);
	}
}

/**
 * buffer_cat - append string at end of buffer
 * @buffer: buffer to be filled
 * @str_to_add: string to be put in buffer
 * Return: void, set errno.
 */
int buffer_cat(char *buffer, char *str_to_add)
{
	int len, i;

	len = _strlen(buffer);
	for (i = 0; str_to_add[i]; i++)
	{
		buffer[len + i] = str_to_add[i];
	}
	buffer[len + i] = '\0';
	return (len + i);
}
