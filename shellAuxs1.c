#include "shell.h"

/**
 * _strchr - Locates character in string.
 *
 * @s: Pointer to string where character is located from.
 * @c: Character to be located.
 *
 * Return: Pointer to first occurrence of @c in @s,
 *	   NULL if character is not found.
 */
char *_strchr(char *s, char c)
{
	unsigned int i = 0;

	for (; *(s + i) != '\0'; i++)
		if (*(s + i) == c)
			return (s + i);
	if (*(s + i) == c)
		return (s + i);
	return ('\0');
}
/**
 * _strdup -  function that duplicates a string
 * in a newly allocated space in memory.
 * @str: string to be copied.
 * Return: Duplicated string in success.
 */

char *_strdup(char *str)
{
	int a = 0, b = 0;
	char *new_s = NULL;

	if (str == NULL)
		return (NULL);

	a = strlen(str);

	new_s = malloc(sizeof(char) * (a + 1));

	if (new_s == NULL)
	{
		perror("./hsh");
		return (NULL);
	}

	if (a == 0 || new_s == 0)
		return (NULL);

	while (b < (a + 1))
	{
		new_s[b] = str[b];
		b++;
	}

	return (new_s);
}
