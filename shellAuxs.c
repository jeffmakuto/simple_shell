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
 * _strstr - Finds the first occurence of the
 *	     substring @needle in string @haystack.
 *
 * @needle: Substring to be located.
 * @haystack: Location of substring @needle.
 *
 * Return: Pointer to the beginning of located substring.
 *	   NULL if substring is not found.
 */
char *_strstr(char *haystack, char *needle)
{
	char *cow;
	char *tayler;

	while (*haystack != '\0')
	{
		cow = haystack;
		tayler = needle;

		while (*haystack != '\0' && *tayler != '\0' && *haystack == *tayler)
		{
			haystack++;
			tayler++;
		}
		if (!*tayler)
			return (cow);
		haystack = cow + 1;
	}
	return (0);
}
/**
 * _atoi - Convert a string to an integer.
 * @s: The pointer to convert
 *
 * Return: A integer
 */
int _atoi(char *s)
{
	int c = 0;
	unsigned int ni = 0;
	int min = 1;
	int isi = 0;

	while (s[c])
	{
		if (s[c] == 45)
		{
			min *= -1;
		}

		while (s[c] >= 48 && s[c] <= 57)
		{
			isi = 1;
			ni = (ni * 10) + (s[c] - '0');
			c++;
		}

		if (isi == 1)
		{
			break;
		}

		c++;
	}

	ni *= min;
	return (ni);
}
