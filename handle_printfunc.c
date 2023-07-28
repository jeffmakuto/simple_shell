#include "shell.h"

/**
 * _print - print array of char to stdout
 * @str: ptr to array printed
 * Return: number of printed bytes,-1 on error
 */
int _print(char *str)
{
	return (write(STDOUT_FILENO, str, _strlen(str)));
}
/**
 * _printe - print array of char to stdout
 * @str: ptr to the array
 * Return: number of bytes printed, -1 on error
 */
int _printe(char *str)
{
	return (write(STDERR_FILENO, str, _strlen(str)));
}

/**
 * _print_error - print array of char in stdo
 * @info: commands passed
 * @err: error code
 * Return: number of bytes printed, -1 on error.
 */
int _print_error(int err, info_t *info)
{
	char n_as_str[10] = {'\0'};

	long_to_string((long) info->count, n_as_str, 10);

	if (err == 2 || err == 3)
	{
		_printe(info->prog_name);
		_printe(": ");
		_printe(n_as_str);
		_printe(": ");
		_printe(info->tokens[0]);
		if (err == 2)
			_printe(": Invalid number: ");
		else
			_printe(": cd error ");
		_printe(info->tokens[1]);
		_printe("\n");
	}
	else if (err == 127)
	{
		_printe(info->prog_name);
		_printe(": ");
		_printe(n_as_str);
		_printe(": ");
		_printe(info->cmd);
		_printe(": not found\n");
	}
	else if (err == 126)
	{
		_printe(info->prog_name);
		_printe(": ");
		_printe(n_as_str);
		_printe(": ");
		_printe(info->cmd);
		_printe(": Permission denied\n");
	}
	return (0);
}
