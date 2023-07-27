#include "shell.h"

/**
 * _envp - show the environment
 *
 * @args: The arguments passed
 *
 * Return: 0 on success
 */
int _envp(PROGARGS *args)
{
	int i;
	char varName[MAX_PATH_LEN] = {'\0'};
	char *varCopy = NULL;

	if (args->tokens[1] == NULL)
		printEnv(args);
	else
	{
		for (i = 0; args->tokens[1][i]; i++)
		{
			if (args->tokens[1][i] == '=')
			{
				varCopy = _strdup(_getenv(varName, args));
				if (varCopy != NULL)
					_setenv(varName, args->tokens[1] + i + 1, args);

				print_env(args);
				if (_getenv(varName, args) == NULL)
				{
					write(STDOUT_FILENO, args->tokens[1], _strlen(args->tokens[1]));
					write(STDOUT_FILENO, "\n", 1);
				}
				else
				{
					_setenv(varName, varCopy, args);
					free(varCopy);
				}
				return (0);
			}
			varName[i] = args->tokens[1][i];
		}
		perror("./hsh:");
	}
	return (0);
}

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
