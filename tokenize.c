#include "shell.h"
/**
 * process_command - this function separate the string using a delimiter
 * @info: a pointer to the program's data
 * Return: an array of the different parts of the string
 */
void process_command(info_t *info)
{
	char *delims = " \t";
	int i, j, count = 2, len;

	len = _strlen(info->buff);
	if (len)
	{
		if (info->buff[len - 1] == '\n')
			info->buff[len - 1] = '\0';
	}

	for (i = 0; info->buff[i]; i++)
	{
		for (j = 0; delims[j]; j++)
		{
			if (info->buff[i] == delims[j])
				count++;
		}
	}

	info->tokens = malloc(count * sizeof(char *));
	if (info->tokens == NULL)
	{
		perror(info->prog_name);
		exit(errno);
	}
	i = 0;
	info->tokens[i] = _strdup(_strtok(info->buff, delims));
	info->cmd = _strdup(info->tokens[0]);
	while (info->tokens[i++])
	{
		info->tokens[i] = _strdup(_strtok(NULL, delims));
	}
}
