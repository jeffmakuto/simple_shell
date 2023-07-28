#include "shell.h"

/**
* _getline - read input.
*
* @args: commands passed
*
* Return: read bytes.
*/
int _getline(prog_args *args)
{
	char buffer[BUFFER_SIZE] = {'\0'};
	static char *commands[10] = {NULL};
	static char operators[10] = {'\0'};
	ssize_t bytes_read, i = 0;

	/* check if there are no more commands in the array */
	/* also check the logical operators */
	if (!commands[0] || (operators[0] == '&' && errno != 0) ||
			(operators[0] == '|' && errno == 0))
	{
		/*free the memory allocated in the array if it exists */
		for (i = 0; commands[i]; i++)
		{
			free(commands[i]);
			commands[i] = NULL;
		}

		/* read from the file descriptor into the buffer */
		bytes_read = read(args->fd, &buffer, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);

		/* check  for '\n' or ; */
		i = 0;
		do {
			commands[i] = _strdup(_strtok(i ? NULL : buffer, "\n;"));
			/*checks and split for && and || operators*/
			i = handle_logical_operators(commands, i, operators);
		} while (commands[i++]);
	}

	/* obtain the next command (command 0) and remove it for the array */
	args->buffer = commands[0];
	for (i = 0; commands[i]; i++)
	{
		commands[i] = commands[i + 1];
		operators[i] = operators[i + 1];
	}

	return (_strlen(args->buffer));
}

/**
 * _atoi - converts string to integer
 *
 * @str: pointer to the string
 *
 * Return: 0, or string int
 */
int _atoi(char *str)
{
	int sign = 1;
	unsigned int num = 0;

	while (!('0' <= *str && *str <= '9') && *str != '\0')
	{
		if (*str == '-')
			sign *= -1;
		if (*str == '+')
			sign *= +1;
		str++;
	}

	while ('0' <= *str && *str <= '9' && *str != '\0')
	{

		num = (num * 10) + (*str - '0');
		str++;
	}
	return (num * sign);
}


/**
 * long_to_str - converts integer to string.
 *
 * @num: integer to convert
 *
 * @str: buffer to save the integer as string.
 *
 * @base: base for conversion
 *
 * Return: void
 */
void long_to_str(long num, char *str, int base)
{
	int i = 0, is_negative = 0;
	long quotient = num;
	char letters[] = {"0123456789abcdefghijklmnopqrstuvwxyz"};

	if (!quotient)
		str[i++] = '0';

	if (str[0] == '-')
		is_negative = 1;

	while (quotient)
	{
		if (quotient < 0)
			str[i++] = letters[-(quotient % base)];
		else
			str[i++] = letters[quotient % base];
		quotient /= base;
	}
	if (is_negative)
		str[i++] = '-';

	str[i] = '\0';
	rev_str(str);
}

/**
 * rev_str - reverses a string.
 *
 * @str: pointer to string.
 *
 * Return: void.
 */
void rev_str(char *str)
{

	int i = 0, len = _strlen(str) - 1;
	char hold;

	while (i < len)
	{
		hold = str[i];
		str[i++] = str[len];
		str[len--] = hold;
	}
}

/**
 * _strlen - returns length of a string
 *
 * @str: string to get length of
 *
 * Return: length of @str
 */

int _strlen(char *str)
{
	int len = 0;

	while (*str++)
	{
		len++;
	}

	return (len);
}

