#include "shell.h"

/**
 * _strlen - return length of string
 * @str: ptr to string
 * Return: length of str
 */
int _strlen(char *str)
{
	int len = 0;

	if (str == NULL)
		return (0);

	while (str[len++] != '\0')
	{
	}
	return (--len);
}

/**
 * _strdup - it duplicates the string
 * @str: str to be duplicated
 * Return: ptr to the array
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
 * _strncmp - Compare strings
 * @str1: 1st String
 * @str2: s2nd String
 * @num: num of char to be compared, 0 if infinite
 * Return: 1 if the strings are similar, 0 if the strings not
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
 * @str1: 1st String
 * @str2: 2nd String
 *
 * Return: ptr to the str
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
 * rev_str - reverses a string.
 *
 * @str: ptr to str.
 * Return: void.
 */
void rev_str(char *str)
{

	int i = 0, len = _strlen(str) - 1;
	char hold;

	while (i < len)
	{
		hold = str[i];
		str[i++] = str[len];
		str[len--] = hold;
	}
}
