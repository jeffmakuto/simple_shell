#include "shell.h"

/**
 * long_to_str - convert integer to a string.
 * @num: integer to concert
 * @str: buffer to save the integer as string.
 * @base: base for conversion
 *
 * Return: void
 */
void long_to_string(long num, char *str, int base)
{
	int i = 0, is_negative = 0;
	long quotient = num;
	char letters[] = {"0123456789abcdef"};

	if (quotient == 0)
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
 * _atoi - converts string to integer
 *
 * @str: pointer to string
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

