/**
 * countChars - count the number of characters in a string.
 *
 * @str: pointer to string
 *
 * @chars: string to be counted
 *
 * Return: 0, or string integer
 */
int countChars(char *str, char *chars)
{
	int i = 0, count = 0;

	for (; str[i]; i++)
	{
		if (str[i] == chars[0])
			count++;
	}
	return (count);
}
