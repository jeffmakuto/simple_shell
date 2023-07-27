#include "shell.h"

/**
 * _strdup -  function that duplicates a string
 * in a newly allocated space in memory.
 *
 * @str: string to be copied.
 *
 * Return: Duplicated string in success.
 */

char *_strdup(char *str)
{
	int a, b;
	char *new_s;

	if (str == NULL)
		return (NULL);

	a = _strlen(str) + 1;

	new_s = malloc(sizeof(char) * (a));

	if (new_s == NULL)
	{
		perror("./hsh");
		return (NULL);
	}

	for (b = 0; b < a; b++)
		new_s[b] = str[b];

	return (new_s);
}

/**
 * _strtok - separates strings with delimiters
 *
 * @line: It´s pointer to array we receive in getline.
 *
 * @delim: It´s characters we mark off string in parts.
 *
 * Return: A pointer to the created token
*/
char *_strtok(char *line, char *delim)
{
	int j;
	static char *str;
	char *copystr;

	if (line != NULL)
		str = line;
	for (; *str != '\0'; str++)
	{
		for (j = 0; delim[j] != '\0'; j++)
		{
			if (*str == delim[j])
			break;
		}
		if (delim[j] == '\0')
			break;
	}
	copystr = str;
	if (*copystr == '\0')
		return (NULL);
	for (; *str != '\0'; str++)
	{
		for (j = 0; delim[j] != '\0'; j++)
		{
			if (*str == delim[j])
			{
				*str = '\0';
				str++;
				return (copystr);
			}
		}
	}
	return (copystr);
}

