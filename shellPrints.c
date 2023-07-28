#include "shell.h"

/**
 * _print - prints array of characters to stdout
 *
 * @str: chars to print
 *
 * Return: number of printed bytes,-1 on error
 */
int _print(char *str)
{
	return (write(STDOUT_FILENO, str, _strlen(str)));
}
/**
 * _printe - prints error to stdout
 *
 * @str: errors to print
 *
 * Return: number of bytes printed
 */
int _printe(char *str)
{
	return (write(STDERR_FILENO, str, _strlen(str)));
}

/**
 * printErr - prints error codes with their meaning
 *
 * @args: commands passed
 *
 * @err: the error code
 *
 * Return: numbers of bytes printed
 */
int printErr(int err, PROGARGS *args)
{
	char numToStr[INITIAL_ENVP_SIZE] = {'\0'};

	longToStr((long) args->count, numToStr, INITIAL_ENVP_SIZE);

	if (err == 2 || err == 3)
	{
		_printe(args->progName);
		_printe(": ");
		_printe(numToStr);
		_printe(": ");
		_printe(args->tokens[0]);
		if (err == 2)
			_printe(": Invalid number: ");
		else
			_printe(": cd error ");
		_printe(args->tokens[1]);
		_printe("\n");
	}
	else if (err == 127)
	{
		_printe(args->progName);
		_printe(": ");
		_printe(numToStr);
		_printe(": ");
		_printe(args->cmd);
		_printe(": not found\n");
	}
	else if (err == 126)
	{
		_printe(args->progName);
		_printe(": ");
		_printe(numToStr);
		_printe(": ");
		_printe(args->cmd);
		_printe(": Permission denied\n");
	}
	return (0);
}
