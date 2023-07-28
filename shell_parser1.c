#include "shell.h"

/**
 * replace_path_variable - Replace the $PATH variable in the string
 *
 * @input: The input string
 *
 * @envp: The environment variables
 *
 * Return: A new string with the $PATH variable replaced.
 * Note: The returned string should be freed by the caller.
 */
char *replace_path_variable(char *input, char **envp)
{
	char *replaced_str = _strdup(input), *ptr = replaced_str, *path_env, *new_str;
	size_t path_len, replace_len, new_len;

	/* Handle $PATH */
	if (_strstr(replaced_str, "$PATH"))
	{
		path_env = _getenv("PATH", envp);
		if (path_env)
		{
			ptr = _strstr(replaced_str, "$PATH");
			path_len = _strlen(path_env);
			replace_len = _strlen("$PATH");
			new_len = _strlen(replaced_str) - replace_len + path_len;
			new_str = malloc(new_len + 1);
			if (!new_str)
			{
				perror("./hsh: malloc error");
				free(replaced_str);
				return (NULL);
			}
			/* Copy the part before $PATH */
			_strncpy(new_str, replaced_str, ptr - replaced_str);
			new_str[ptr - replaced_str] = '\0';
			/* Concatenate the actual PATH value */
			_strcat(new_str, path_env);
			/* Concatenate the part after $PATH */
			_strcat(new_str, ptr + replace_len);
			/* Free the old replacedStr and use the new string */
			free(replaced_str);
			replaced_str = new_str;
		}
	}
	return (replaced_str);
}

/**
 * int_to_string - Converts an integer to its string representation.
 *
 * @num: The integer to convert.
 *
 * @str: The buffer to store the resulting string.
 *
 * Return: A pointer to the buffer containing the string representation.
 */
char *int_to_string(int num, char *str)
{
	int i = 0, is_negative = 0, rem, j, len;
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
		is_negative = 1;
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
	if (is_negative)
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
 * concat_strings - Concatenates two strings and returns a new dynamically
 * allocated string.
 *
 * @str1: The first string.
 *
 * @str2: The second string.
 *
 * Return: A pointer to the dynamically allocated concatenated string.
 */
char *concat_strings(char *str1, char *str2)
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
 * replace_variables - Replaces specific variables in the input string with
 * their values.
 *
 * @input: The input string to process.
 *
 * @envp: The environment variables
 *
 * Return: A pointer to the new dynamically allocated string with replaced
 * variables.
 */
char *replace_variables(char *input, char **envp)
{
	char *replaced_str = _strdup(input), *ptr = replaced_str, *temp;
	char pid_str[20]; /* Buffer to hold the process ID string */
	int last_exit_status = 0;

	/* Replace $? with the exit status of the last command */
	if (_strstr(replaced_str, "$?"))
	{
		int_to_string(last_exit_status, pid_str);
		ptr = _strstr(replaced_str, "$?");
		*ptr = '\0';
		temp = concat_strings(replaced_str, pid_str);
		free(replaced_str);
		replaced_str = temp;
		ptr = replaced_str + _strlen(replaced_str); /* Update pointer to end of str */
		_strcat(replaced_str, ptr + 2);
	}

	/* Replace $$ with the shell's process ID */
	if (_strstr(replaced_str, "$$"))
	{
		int_to_string(getpid(), pid_str);
		ptr = _strstr(replaced_str, "$$");
		*ptr = '\0';
		temp = concat_strings(replaced_str, pid_str);
		free(replaced_str);
		replaced_str = temp;
		ptr = replaced_str + _strlen(replaced_str); /* Update pointer to end of str */
		_strcat(replaced_str, ptr + 2);
	}

	/* Replace $PATH using the separate function (replace_path_variable) */
	temp = replace_path_variable(replaced_str, envp);
	free(replaced_str); /* Free previous replacedStr before reassigning pointer */
	replaced_str = temp;

	return (replaced_str);
}

