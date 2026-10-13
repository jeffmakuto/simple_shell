#include "shell.h"

/**
 * is_name_char - check if a char can be part of a variable name
 * @c: char to check
 * Return: 1 if it can, 0 otherwise
 */
static int is_name_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') || c == '_');
}

/**
 * get_value - get the value of the variable at the start of str
 * @str: string starting just after the '$'
 * @value: buffer of BUFFER_SIZE bytes filled with the value
 * @info: commands passed
 * Return: number of chars of the variable name, 0 if there is none
 */
static int get_value(char *str, char *value, info_t *info)
{
	int j;
	char *temp;

	value[0] = '\0';
	if (str[0] == '?' || str[0] == '$')
	{
		long_to_string(str[0] == '?' ? (long)errno : (long)getpid(),
			value, 10);
		return (1);
	}
	for (j = 0; is_name_char(str[j]); j++)
		value[j] = str[j];
	value[j] = '\0';
	if (j == 0)
		return (0);
	temp = _getenv(value, info);
	value[0] = '\0';
	if (temp)
		buffer_cat(value, temp);
	return (j);
}

/**
 * expand_variables - expand the variable
 * @info: commands passed
 *
 * Return: void
 */
void expand_variables(info_t *info)
{
	int i, j;
	char line[BUFFER_SIZE] = {'\0'}, rest[BUFFER_SIZE], value[BUFFER_SIZE];

	if (info->buff == NULL)
		return;
	buffer_cat(line, info->buff);
	for (i = 0; line[i]; i++)
	{
		if (line[i] != '$')
			continue;
		j = get_value(line + i + 1, value, info);
		if (j == 0)
			continue;
		rest[0] = '\0';
		buffer_cat(rest, line + i + j + 1);
		line[i] = '\0';
		/* continue after the inserted value, it's not expanded again */
		i = buffer_cat(line, value) - 1;
		buffer_cat(line, rest);
	}
	if (!_strncmp(info->buff, line, 0))
	{
		free(info->buff);
		info->buff = _strdup(line);
	}
}

/**
 * buffer_cat - append string at end of buffer
 * @buffer: buffer of BUFFER_SIZE bytes to be filled
 * @str_to_add: string to be put in buffer
 * Return: length of the buffer, str_to_add is cut if it doesn't fit.
 */
int buffer_cat(char *buffer, char *str_to_add)
{
	int len, i;

	len = _strlen(buffer);
	for (i = 0; str_to_add[i] && len + i < BUFFER_SIZE - 1; i++)
	{
		buffer[len + i] = str_to_add[i];
	}
	buffer[len + i] = '\0';
	return (len + i);
}
