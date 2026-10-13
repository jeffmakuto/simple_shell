#include "shell.h"

/**
 * read_line - read one line from a file descriptor
 * @fd: file descriptor to read from
 * @line: buffer of BUFFER_SIZE bytes where the line is stored
 * Return: number of chars stored, -1 on EOF with nothing read
 */
static int read_line(int fd, char *line)
{
	static char buf[BUFFER_SIZE];
	static ssize_t len, pos;
	int n = 0;

	while (n < BUFFER_SIZE - 1)
	{
		if (pos >= len)
		{
			pos = 0;
			len = read(fd, buf, BUFFER_SIZE);
			if (len <= 0)
			{
				len = 0;
				break;
			}
		}
		line[n] = buf[pos++];
		if (line[n++] == '\n')
			break;
	}
	line[n] = '\0';
	return (n == 0 ? -1 : n);
}

/**
 * is_blank - check if a string only contains spaces and tabs
 * @str: string to check
 * Return: 1 if blank, 0 otherwise
 */
static int is_blank(char *str)
{
	for (; *str; str++)
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
	return (1);
}

/**
 * split_line - split a line in commands on '\n', ';', "&&" and "||"
 * and remove the comments
 * @line: line to split (it's modified)
 * @commands: array filled with the commands, NULL terminated
 * @operators: logical operator ('\0', '&' or '|') before each command
 */
static void split_line(char *line, char *commands[], char operators[])
{
	int i, n = 0;
	char *start = line, op = '\0', c;

	for (i = 0; ; i++)
	{
		if (line[i] == '#' && (i == 0 || line[i - 1] == ' ' ||
			line[i - 1] == '\t' || line[i - 1] == '\0'))
			line[i] = '\0';
		c = line[i];
		if (c != '\0' && c != '\n' && c != ';' &&
			!((c == '&' || c == '|') && line[i + 1] == c))
			continue;
		line[i] = '\0';
		if (!is_blank(start))
		{
			commands[n] = _strdup(start);
			operators[n++] = op;
		}
		if (c == '\0')
			break;
		op = (c == '&' || c == '|') ? c : '\0';
		if (op)
			line[++i] = '\0';
		start = line + i + 1;
	}
	commands[n] = NULL;
}

/**
 * _getline - get the next command to execute
 * @info: commands passed
 * @prompt: prompt printed before reading a new line, can be NULL
 * Return: length of the command, -1 on EOF.
 */
int _getline(info_t *info, char *prompt)
{
	static char *commands[BUFFER_SIZE];
	static char operators[BUFFER_SIZE];
	static int next;
	char line[BUFFER_SIZE];

	/* skip the commands whose && or || condition is not met */
	while (commands[next] && ((operators[next] == '&' && errno != 0) ||
		(operators[next] == '|' && errno == 0)))
		free(commands[next++]);

	if (!commands[next])
	{
		next = 0;
		_print(prompt);
		if (read_line(info->fd, line) == -1)
			return (-1);
		info->count++;
		split_line(line, commands, operators);
		if (!commands[0])
			return (0);
	}

	info->buff = commands[next++];
	return (_strlen(info->buff));
}
