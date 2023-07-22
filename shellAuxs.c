#include "shell.h"
/**
 * _strlen - returns length of a string
 * @s: string to get length of
 *
 * Return: length of @s
 */

int _strlen(char *s)
{
	int length = 0;

	while (*s++)
	{
		length++;
	}

	return (length);
}
/**
 * _strcmp - function compares two strings.
 * @s1: first string to compare.
 * @s2: second string to compare.
 *
 * Return: If s1 is less than s2 (less than 0),
 * If s1 equal to s2 (0), s1 is greater than s2 (greater than 0).
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
		{
			return (0);
		}
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
/**
 * _strspn - gets length of a prefix substring
 *
 * @s: segment where the bytes are gotten from
 * @accept: source of bytes used in s
 *
 * Return: number of bytes in s consisting of only those
 *         from accept
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int i, j, x;

	for (i = 0; *(s + i) != '\0'; i++)
	{
		x = 1;
		for (j = 0; *(accept + j) != '\0'; j++)
		{
			if (*(s + i) == *(accept + j))
			{
				x = 0;
				break;
			}
		}
		if (x == 1)
			break;
	}
	return (i);
}
