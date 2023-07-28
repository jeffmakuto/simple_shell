#include "shell.h"

/**
 * _strdup - it duplicates the string
 *
 * @str: string to be duplicated
 *
 * Return: pointer to @str
 */
char *_strdup(char *str)
{
	char *result;
	int len, i;

	if (str == NULL)
		return (NULL);

	len = _strlen(str) + 1;

	result = malloc(sizeof(char) * len);

	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}
	for (i = 0; i < len ; i++)
	{
		result[i] = str[i];
	}

	return (result);
}

/**
 * _strncmp - compares 2  strings of a certain size
 *
 * @st1: 1st string
 *
 * @str2: 2nd string
 *
 * @num: number of chars to be compared, 0 if infinite
 *
 * Return: 1 if the strings are similar, 0 if the strings are not
 */
int _strncmp(char *str1, char *str2, int num)
{
	int i;

	if (str1 == NULL && str2 == NULL)
		return (1);

	if (str1 == NULL || str2 == NULL)
		return (0);

	if (num == 0)
	{
		if (_strlen(str1) != _strlen(str2))
			return (0);
		for (i = 0; str1[i]; i++)
		{
			if (str1[i] != str2[i])
				return (0);
		}
		return (1);
	}
	else
	{
		for (i = 0; i < num ; i++)
		{
			if (str1[i] != str2[i])
			return (0);
		}
		return (1);
	}
}

/**
 * _strcat - concatenate strings.
 *
 * @str1: 1st string
 *
 * @str2: 2nd string
 *
 * Return: pointer to the string
 */
char *_strcat(char *str1, char *str2)
{
	char *result;
	int len1 = 0, len2 = 0;

	if (str1 == NULL)
		str1 = "";
	len1 = _strlen(str1);

	if (str2 == NULL)
		str2 = "";
	len2 = _strlen(str2);

	result = malloc(sizeof(char) * (len1 + len2 + 1));
	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}

	for (len1 = 0; str1[len1] != '\0'; len1++)
		result[len1] = str1[len1];
	free(str1);

	for (len2 = 0; str2[len2] != '\0'; len2++)
	{
		result[len1] = str2[len2];
		len1++;
	}

	result[len1] = '\0';
	return (result);
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
