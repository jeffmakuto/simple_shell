#include "shell.h"

/**
 * setenvAction - Sets environment
 *
 * @args: Arguments passed
 *
 * Return: 0 on sucess
 */
int setenvAction(PROGARGS *args)
{
	if (args->tokens[1] == NULL || args->tokens[2] == NULL)
		return (0);
	if (args->tokens[3] != NULL)
	{
		perror("./hsh: Error: Too many arguments");
		return (1);
	}

	_setenv(args->tokens[1], args->tokens[2], args);

	return (0);
}

/**
 * unsetenvAction - Unset environment
 * 
 * @args: Arguments passed
 *
 * Return: 0 on success
 */
int unsetenvAction(PROGARGS *args)
{
	if (args->tokens[1] == NULL)
		return (0);
	if (args->tokens[2] != NULL)
	{
		perror("./hsh: Error: Too many arguments");
		return (1);
	}
	_unsetenv(args->tokens[1], args);

	return (0);
}
