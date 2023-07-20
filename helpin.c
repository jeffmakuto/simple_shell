#include "shell.h"

/**
 * _strlen - function that calculates the length of a string.
 * @str: The input string.
 * Return: The length of the string (excluding the null terminator).
 */
int _strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

/**
 * _strcat - function that concatenates two strings.
 * @dest: The destination buffer to hold the concatenated string.
 * @src: The source string to be appended to the destination.
 * Return: A pointer to the concatenated string (same as dest).
 */
char *_strcat(char *dest, const char *src)
{
    int dest_len = _strlen(dest);
    int i;

    for (i = 0; src[i] != '\0'; i++)
    {
        dest[dest_len + i] = src[i];
    }

    dest[dest_len + i] = '\0';

    return dest;
}

