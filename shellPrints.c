#include "shell.h"

/**
 * _print - Print array of char in stdout
 *
 * @str: Pointer to array printed
 *
 * Return: num of printed bytes,-1 on error
 */
int _print(char *str)
{
	return (write(STDOUT_FILENO, str, _strlen(str)));
}

/**
 * _printe - Print array of chaar in stdout
 *
 * @str: Pointer to the array
 *
 * Return: num of bytes printed, -1 on error
 */
int _printe(char *str)
{
	return (write(STDERR_FILENO, str, _strlen(str)));
}

/**
 * printErr - print array of char in stdout
 *
 * @str: Pointer to the array
 *
 * @errCode: error code
 *
 * Return: num of bytes printed, -1 on error.
 */
int printErr(int errCode, PROGARGS *args)
{
	char n_as_string[INITIAL_ENVP_SIZE] = {'\0'};

	longToStr((long) args->execCount, n_as_string, INITIAL_ENVP_SIZE);

	if (errCode == 2 || errCode == 3)
	{
		_printe(args->programName);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(args->tokens[0]);
		if (errCode == 2)
			_printe(": Illegal number: ");
		else
			_printe(": can't cd to ");
		_printe(args->tokens[1]);
		_printe("\n");
	}
	else if (errCode == 127)
	{
		_printe(args->programName);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(args->cmd);
		_printe(": not found\n");
	}
	else if (errCode == 126)
	{
		_printe(args->programName);
		_printe(": ");
		_printe(n_as_string);
		_printe(": ");
		_printe(args->cmd);
		_printe(": Permission denied\n");
	}
	return (0);
}
