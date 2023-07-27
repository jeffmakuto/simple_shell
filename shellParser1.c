#include "shell.h"

/**
 * replaceVariables - Expands the variable
 *
 * @args: Arguments passed
 *
 * Return: Void
 */
void replaceVariables(PROGARGS *args)
{
	int i, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (args->buffer == NULL)
		return;
	_strcat(line, args->buffer);
	for (i = 0; line[i]; i++)
		if (line[i] == '#')
			line[i--] = '\0';
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			long_to_string(errno, expansion, 10);
			_strcat(line, expansion);
			_strcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			long_to_string(getpid(), expansion, 10);
			_strcat(line, expansion);
			_strcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		else if (line[i] == '$')
		{
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			temp = _getenv(expansion, args);
			line[i] = '\0', expansion[0] = '\0';
			_strcat(expansion, line + i + j);
			temp ? _strcat(line, temp) : 1;
			_strcat(line, expansion);
		}
	if (!_strcmp(args->buffer, line, 0))
	{
		free(args->buffer);
		args->buffer = _strdup(line);
	}
}

