#include "shell.h"

/**
 * _realloc - Reallocates a memory block.
 * @ptr: Pointer to the existing memory block.
 * @new_size: The new size to allocate.
 *
 * Return: On success, returns a pointer to the newly reallocated memory block.
 * On failure, returns NULL.
 */
void *_realloc(void *ptr, size_t new_size)
{
	void *new_ptr;
	size_t min_size, i;

	/* If ptr is NULL, the behavior of _realloc is same as malloc(new_size)*/
	if (!ptr)
		return (malloc(new_size));

	/* If new_size is 0, the behavior of _realloc is same as free(ptr) */
	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}

	/* Allocate a new memory block with the new size */
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		/* If malloc fails, return NULL indicating failure to reallocate */
		return (NULL);
	}

	/* Determine the minimum size to copy from the old block to the new block */
	/* It should be the smaller of the old and new sizes */
	min_size = new_size;
	if (min_size > sizeof(ptr))
		min_size = sizeof(ptr);

	/* Copy the data from the old block to the new block */
	for (i = 0; i < min_size; i++)
		*((char *)new_ptr + i) = *((char *)ptr + i);

	/* Free the old block */
	free(ptr);
	return (new_ptr);
}

/**
 * _getenv - retrieves the value of the environment variable specified by
 * the 'name' argument.
 *
 * @name: The name of the environment variable to retrieve.
 *
 * @envp: The environment variables
 *
 * Return: If the environment variable 'name' is found, the function returns
 * a pointer to its value. If the variable is not found or an error
 * occurs, it returns NULL.
 */

char *_getenv(char *name, char **envp)
{
	size_t name_len;
	char **env_var;

	if (name == NULL || *name == '\0' || _strchr(name, '=') != NULL)
	{
		/* Invalid environment variable name or contains '='. */
		return (NULL);
	}

	name_len = _strlen(name);

	for (env_var = envp; *env_var != NULL; env_var++)
	{
		if (_strncmp(*env_var, name, name_len) == 0 && (*env_var)[name_len] == '=')
		{
			/* Found the matching environment variable. */
			return (*env_var + name_len + 1);
		}
	}

	/* The environment variable 'name' was not found. */
	return (NULL);
}

/**
 * _strncmp - Compare two strings up to a maximum number of characters.
 * @str1: Pointer to the first string.
 * @str2: Pointer to the second string.
 * @n: Number of characters to compare.
 *
 * This function compares the first @n characters of two strings
 * @str1 and @str2.
 *
 * Return: An integer less than, equal to, or greater than 0 if @str1 is found,
 * respectively, to be less than, to match, or be greater than @str2.
 */
int _strncmp(const char *str1, const char *str2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
	{
		/* Check if we reached the end of either string */
		if (str1[i] == '\0' || str2[i] == '\0')
		{
			/* If one of the strings ended early, we compare their lengths */
			if (i == n - 1)
				return (0); /* Both strings have the same first n characters */
			else
				return (str1[i] - str2[i]);
		}

		/* Compare the characters */
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
	}

	/* If we reach this point, the first n characters are the same */
	return (0);
}

/**
 * _strcspn - Calculate the length of the initial substring without reject
 * characters.
 * @str: The input string.
 * @reject: The string containing characters to reject.
 *
 * This function calculates the length of the initial substring of 'str'
 * that does not contain any characters present in 'reject'. It searches
 * for the first occurrence of any character from 'reject' in 'str', and
 * returns the number of characters from the beginning of 'str' until that
 * occurrence is found.
 *
 * Return: The length of the initial substring without any reject characters.
 */
size_t _strcspn(const char *str, const char *reject)
{
	size_t count = 0;
	char ch;
	const char *ptr;

	/* Loop through the string until a null terminator is encountered */
	while ((ch = str[count]) != '\0')
	{
		/* Check if the current character is present in the reject string */
		ptr = reject;
		while (*ptr != '\0')
		{
			if (*ptr == ch)
				return (count); /* Return the count if any character is found in reject */
			ptr++;
		}
		count++; /* Move to the next character in the string */
	}
	return (count); /* If no char from reject is found, return length of string */
}

/**
 * _strtok_r - Custom reentrant string tokenizer function.
 * @str: The string to be tokenized. If NULL, continue from previous save_ptr.
 * @delimiters: A string containing delimiter characters.
 * @save_ptr: A pointer to a char* variable that holds the position in
 * original string between successive calls to _strtok_r. Should be set to
 * NULL on 1st call.
 *
 * This function works like strtok_r from the standard library, tokenizing the
 * input string based on the given delimiters.
 *
 * Return: A pointer to next token found, or NULL if no more tokens are
 * present.
 */
char *_strtok_r(char *str, const char *delimiters, char **save_ptr)
{
	char *token_end;

	/* If str is NULL, continue from the previous save_ptr */
	if (str == NULL)
		str = *save_ptr;

	/* Skip leading delimiters */
	str += _strspn(str, delimiters);

	/* If we are at the end of the string, return NULL */
	if (*str == '\0')
	{
		*save_ptr = str;
		return (NULL);
	}

	/* Find the end of the token */
	token_end = str + _strcspn(str, delimiters);

	/* If the token is not empty, terminate it and update save_ptr */
	if (*token_end != '\0')
	{
		*token_end = '\0';
		*save_ptr = token_end + 1;
	}
	else
		*save_ptr = token_end;

	return (str);
}
