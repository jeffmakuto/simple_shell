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
 * @Return: If the environment variable 'name' is found, the function returns
 * a pointer to its value. If the variable is not found or an error
 * occurs, it returns NULL.
 */
char *_getenv(char *name)
{
	extern char **environ;
	size_t name_len;
	char **env_var;

	if (name == NULL || *name == '\0' || _strchr(name, '=') != NULL)
	{
		/* Invalid environment variable name or contains '='. */
		return (NULL);
	}

	name_len = _strlen(name);

	for (env_var = environ; *env_var != NULL; env_var++)
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
 * custom_strncmp - Compare two strings up to a maximum number of characters.
 * @str1: Pointer to the first string.
 * @str2: Pointer to the second string.
 * @n: Number of characters to compare.
 *
 * This function compares the first @n characters of two strings @str1 and @str2.
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
	return (count); /* If no character from reject is found, return the length of the string */
}
