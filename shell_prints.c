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
 * print_err - prints error codes with their meaning
 *
 * @args: commands passed
 *
 * @err: the error code
 *
 * Return: numbers of bytes printed
 */
int print_err(int err, prog_args *args)
{
	char num_to_str[INITIAL_ENVP_SIZE] = {'\0'};

	long_to_str((long) args->count, num_to_str, INITIAL_ENVP_SIZE);

	if (err == 2 || err == 3)
	{
		_printe(args->prog_name);
		_printe(": ");
		_printe(num_to_str);
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
		_printe(args->prog_name);
		_printe(": ");
		_printe(num_to_str);
		_printe(": ");
		_printe(args->cmd);
		_printe(": not found\n");
	}
	else if (err == 126)
	{
		_printe(args->prog_name);
		_printe(": ");
		_printe(num_to_str);
		_printe(": ");
		_printe(args->cmd);
		_printe(": Permission denied\n");
	}
	return (0);
}
