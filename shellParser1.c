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
	char *replacedStr = strdup(input), *ptr = replacedStr, *pathEnv, *newStr;
	size_t pathLen, replaceLen, newLen;

	/* Handle $PATH */
	if (strstr(replacedStr, "$PATH"))
	{
		pathEnv = getenv("PATH");
		if (pathEnv)
		{
			ptr = strstr(replacedStr, "$PATH");
			pathLen = strlen(pathEnv);
			replaceLen = strlen("$PATH");
			newLen = strlen(replacedStr) - replaceLen + pathLen;
			newStr = malloc(newLen + 1);
			if (!newStr)
			{
				perror("./hsh: malloc error");
				free(replacedStr);
				return (NULL);
			}
			/* Copy the part before $PATH */
			strncpy(newStr, replacedStr, ptr - replacedStr);
			newStr[ptr - replacedStr] = '\0';
			/* Concatenate the actual PATH value */
			strcat(newStr, pathEnv);
			/* Concatenate the part after $PATH */
			strcat(newStr, ptr + replaceLen);
			/* Free the old replacedStr and use the new string */
			free(replacedStr);
			replacedStr = newStr;
		}
	}
	return (replacedStr);
}

/**
 * replaceVariables - Replace variables in a string
 *
 * @input: The input string
 *
 * Return: A new string with variables replaced.
 * Note: The returned string should be freed by the caller.
 */
char *replaceVariables(char *input)
{
	char *replacedStr = strdup(input), *ptr = replacedStr;
	char pidStr[20]; /* Buffer to hold the process ID string */
	int lastExitStatus = 0;

	/* Replace $? with the exit status of the last command */
	if (strstr(replacedStr, "$?"))
	{
		snprintf(pidStr, sizeof(pidStr), "%d", lastExitStatus);
		ptr = strstr(replacedStr, "$?");
		*ptr = '\0';
		strcat(replacedStr, pidStr);
		strcat(replacedStr, ptr + 2);
	}
	/* Replace $$ with the shell's process ID */
	if (strstr(replacedStr, "$$"))
	{
		snprintf(pidStr, sizeof(pidStr), "%d", getpid());
		ptr = strstr(replacedStr, "$$");
		*ptr = '\0';
		strcat(replacedStr, pidStr);
		strcat(replacedStr, ptr + 2);
	}
	/* Replace $PATH using the separate function */
	replacedStr = replacePathVariable(replacedStr);
	return (replacedStr);
}

