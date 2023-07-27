#include "shell.h"

/**
 * _strlen - returns length of a string
 *
 * @str: string to get length of
 *
 * Return: length of @s
 */

int _strlen(char *str)
{
	int len = 0;

	while (*str++)
	{
		len++;
	}

	return (len);
}


/**
 * _strcat - Concatenate 2 strings.
 *
 * @str1: String 1
 *
 * @str2: String 2
 *
 * Return: pointer to the str
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
 * _strncmp - Compares 2 strings
 *
 * @str1: 1st string
 *
 * @str2: 2nd string
 *
 * @n: num of chars to be compared, 0 if infinite
 *
 * Return: 1 if the strings are similar, 0 if the strings not
 */
int _strncmp(char *str1, char *str2, int n)
{
	int i;

	if (!str1 && !str2)
		return (1);

	if (!str1 || !str2)
		return (0);

	if (n == 0)
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
		for (i = 0; i < n ; i++)
		{
			if (str1[i] != str2[i])
			return (0);
		}
		return (1);
	}
}

/**
 * rev_str - reverses a string
 *
 * @str: the reference to the string
 */
void rev_str(char *str)
{
	int len = 0, i, mid, a;
	char c = *str, temp;

	/* get string lenth */
	while (c != '\0')
	{
		c = *(str + ++len);
	}

	/* end if string lenth is zero */
	if (len == 0)
	{
		return;
	}

	mid = len / 2;


	/* reverse string */
	for (i = 0; i < mid; i++)
	{
		a = len - i - 1;

		temp = *(str + i);
		*(str + i) = *(str + a);
		*(str + a) = temp;
	}
}

/**
 * longToStr - convert number to a string.
 *
 * @num: number to concert
 *
 * @str: buffer to save the num as string.
 *
 * @base: base for concersion
 *
 * Return: Nothing.
 */
void longToStr(long num, char *str, int base)
{
	int i = 0, isNegative = 0;
	long quotient = num;
	char letters[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (!quotient)
		str[i++] = '0';

	if (str[0] == '-')
		isNegative = 1;

	while (quotient)
	{
		if (quotient < 0)
			str[i++] = letters[-(quotient % base)];
		else
			str[i++] = letters[quotient % base];
		quotient /= base;
	}
	if (isNegative)
		str[i++] = '-';

	str[i] = '\0';
	rev_str(str);
}
