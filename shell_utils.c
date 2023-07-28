#include "shell.h"

/**
 * execute_command - execute string of arguments
 *
 * @args: arguments passed
 *
 * Return: 0 on success, -1 on fail
 */
int execute_command(prog_args *args)
{
	int result = 0, status;
	pid_t pid;

	result = check_builtins(args);
	if (result != -1)
		return (result);

	result = find_executable(args);
	if (result)
		return (result);

	pid = fork();

	if (pid == -1)
	{
		perror(args->cmd);
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		result = execve(args->tokens[0], args->tokens, args->envp);
			if (result == -1)
			{
				perror(args->cmd);
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			errno = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			errno = 128 + WTERMSIG(status);
	}
	return (0);
}

/**
 * process_command - processes an array of string commands into tokens
 *
 * @args: commands passed
 *
 * Return: tokens
 */
void process_command(prog_args *args)
{
	char *delims = " \t";
	int i, j, count = 2, len;

	len = _strlen(args->buffer);
	if (len)
	{
		if (args->buffer[len - 1] == '\n')
			args->buffer[len - 1] = '\0';
	}

	for (i = 0; args->buffer[i]; i++)
	{
		for (j = 0; delims[j]; j++)
		{
			if (args->buffer[i] == delims[j])
				count++;
		}
	}

	args->tokens = malloc(count * sizeof(char *));
	if (args->tokens == NULL)
	{
		perror(args->prog_name);
		exit(errno);
	}
	i = 0;
	args->tokens[i] = _strdup(_strtok(args->buffer, delims));
	args->cmd = _strdup(args->tokens[0]);
	while (args->tokens[i++])
	{
		args->tokens[i] = _strdup(_strtok(NULL, delims));
	}
}

/**
 * get_path - tokenize path in directories
 *
 * @args: commands passed
 *
 * Return: array of path dirs
 */

char **get_path(prog_args *args)
{
	int i = 0;
	int count_dirs = 2;
	char **tokens = NULL;
	char *PATH;

	PATH = _getenv("PATH", args);
	if ((PATH == NULL) || PATH[0] == '\0')
	{
		return (NULL);
	}

	PATH = _strdup(PATH);

	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			count_dirs++;
	}

	tokens = malloc(sizeof(char *) * count_dirs);

	i = 0;
	tokens[i] = _strdup(_strtok(PATH, ":"));
	while (tokens[i++])
	{
		tokens[i] = _strdup(_strtok(NULL, ":"));
	}

	free(PATH);
	PATH = NULL;
	return (tokens);

}

/**
 * check_file - checks if a file exists, it's not a dir and
 * has excecution permisions for permisions.
 *
 * @file_path: pointer to full file name
 *
 * Return: 0 success, error code on fail
 */

int check_file(char *file_path)
{
	struct stat stat_buff;

	if (stat(file_path, &stat_buff) != -1)
	{
		if (S_ISDIR(stat_buff.st_mode) ||  access(file_path, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}
	errno = 127;
	return (127);
}

/**
 * replace_variables - replaces specific variables in the input string with
 * their values
 *
 * @args: commands passed
 *
 * Return: void
 */
void replace_variables(prog_args *args)
{
	int i, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (args->buffer == NULL)
		return;
	buffcat(line, args->buffer);
	for (i = 0; line[i]; i++)
		if (line[i] == '#')
			line[i--] = '\0';
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			long_to_str(errno, expansion, 10);
			buffcat(line, expansion);
			buffcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			long_to_str(getpid(), expansion, 10);
			buffcat(line, expansion);
			buffcat(line, args->buffer + i + 2);
		}
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		else if (line[i] == '$')
		{
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			temp = _getenv(expansion, args);
			line[i] = '\0', expansion[0] = '\0';
			buffcat(expansion, line + i + j);
			temp ? buffcat(line, temp) : 1;
			buffcat(line, expansion);
		}
	if (!_strncmp(args->buffer, line, 0))
	{
		free(args->buffer);
		args->buffer = _strdup(line);
	}
}
