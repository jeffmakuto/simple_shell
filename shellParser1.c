#include "shell.h"

/**
 * replacePathVariable - Replace the $PATH variable in the string
 *
 * @input: The input string
 *
 * Return: A new string with the $PATH variable replaced.
 * Note: The returned string should be freed by the caller.
 */
char *replacePathVariable(char *input)
{
	char *replacedStr = _strdup(input), *ptr = replacedStr, *pathEnv, *newStr;
	size_t pathLen, replaceLen, newLen;

	/* Handle $PATH */
	if (_strstr(replacedStr, "$PATH"))
	{
		pathEnv = _getenv("PATH");
		if (pathEnv)
		{
			ptr = _strstr(replacedStr, "$PATH");
			pathLen = _strlen(pathEnv);
			replaceLen = _strlen("$PATH");
			newLen = _strlen(replacedStr) - replaceLen + pathLen;
			newStr = malloc(newLen + 1);
			if (!newStr)
			{
				perror("./hsh: malloc error");
				free(replacedStr);
				return (NULL);
			}
			/* Copy the part before $PATH */
			_strncpy(newStr, replacedStr, ptr - replacedStr);
			newStr[ptr - replacedStr] = '\0';
			/* Concatenate the actual PATH value */
			_strcat(newStr, pathEnv);
			/* Concatenate the part after $PATH */
			_strcat(newStr, ptr + replaceLen);
			/* Free the old replacedStr and use the new string */
			free(replacedStr);
			replacedStr = newStr;
		}
	}
	return (replacedStr);
}

/**
 * intToString - Converts an integer to its string representation.
 *
 * @num: The integer to convert.
 *
 * @str: The buffer to store the resulting string.
 *
 * Return: A pointer to the buffer containing the string representation.
 */
char *intToString(int num, char *str)
{
	int i = 0, isNegative = 0, rem, j, len;
	char temp;

	/* Handle 0 explicitly, otherwise, it'll be considered as empty string. */
	if (!num)
	{
		str[i++] = '0';
		str[i] = '\0';
		return (str);
	}

	/* Handle negative numbers */
	if (num < 0)
	{
		isNegative = 1;
		num = -num;
	}

	/* Convert individual digits to characters, add them to string in rev order */
	while (num)
	{
		rem = num % 10;
		str[i++] = rem + '0';
		num = num / 10;
	}
	/* Append '-' if the number is negative */
	if (isNegative)
		str[i++] = '-';
	str[i] = '\0';
	/* Reverse the string to get the correct representation */
	j = 0, len = i - 1;
	while (j < len)
	{
		temp = str[j];
		str[j] = str[len];
		str[len] = temp;
		j++;
		len--;
	}
	return (str);
}

/**
 * concatStrings - Concatenates two strings and returns a new dynamically
 * allocated string.
 *
 * @str1: The first string.
 *
 * @str2: The second string.
 *
 * Return: A pointer to the dynamically allocated concatenated string.
 */
char *concatStrings(char *str1, char *str2)
{
	int len1 = 0, len2 = 0, i;
	char *result;

	/* Find the lengths of the input strings */
	while (str1[len1])
		len1++;
	while (str2[len2])
		len2++;

	/* Allocate memory for the new concatenated string */
	result = (char *)malloc((len1 + len2 + 1) * sizeof(char));

	/* Copy the contents of the first string */
	for (i = 0; i < len1; i++)
		result[i] = str1[i];

	/* Copy the contents of the second string */
	for (i = 0; i < len2; i++)
		result[len1 + i] = str2[i];

	/* Null-terminate the result string */
	result[len1 + len2] = '\0';

	return (result);
}

/**
 * replaceVariables - Replaces specific variables in the input string with
 * their values.
 *
 * @input: The input string to process.
 *
 * Return: A pointer to the new dynamically allocated string with replaced
 * variables.
 */
char *replaceVariables(char *input)
{
	char *replacedStr = _strdup(input), *ptr = replacedStr, *temp;
	char pidStr[20]; /* Buffer to hold the process ID string */
	int lastExitStatus = 0;

	/* Replace $? with the exit status of the last command */
	if (_strstr(replacedStr, "$?"))
	{
		intToString(lastExitStatus, pidStr);
		ptr = _strstr(replacedStr, "$?");
		*ptr = '\0';
		temp = concatStrings(replacedStr, pidStr);
		free(replacedStr);
		replacedStr = temp;
		ptr = replacedStr + _strlen(replacedStr); /* Update pointer to end of str */
		_strcat(replacedStr, ptr + 2);
	}

	/* Replace $$ with the shell's process ID */
	if (_strstr(replacedStr, "$$"))
	{
		intToString(getpid(), pidStr);
		ptr = _strstr(replacedStr, "$$");
		*ptr = '\0';
		temp = concatStrings(replacedStr, pidStr);
		free(replacedStr);
		replacedStr = temp;
		ptr = replacedStr + _strlen(replacedStr); /* Update pointer to end of str */
		_strcat(replacedStr, ptr + 2);
	}

	/* Replace $PATH using the separate function (replacePathVariable) */
	temp = replacePathVariable(replacedStr);
	free(replacedStr); /* Free previous replacedStr before reassigning pointer */
	replacedStr = temp;

	return (replacedStr);
}

