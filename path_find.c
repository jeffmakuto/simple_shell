#include "shell.h"

int check_file(char *full_path);

/**
 * find_executable - look for an executable in path
 * @info: commands passed
 * Return: 0 success, error code on fail
 */

int find_executable(info_t *info)
{
	int i = 0, result = 0;
	char **dirs;

	if (!info->cmd)
		return (2);

	if (info->cmd[0] == '/' || info->cmd[0] == '.')
		return (check_file(info->cmd));

	free(info->tokens[0]);
	info->tokens[0] = _strcat(_strdup("/"), info->cmd);
	if (!info->tokens[0])
		return (2);

	dirs = get_path(info);

	if (!dirs || !dirs[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], info->tokens[0]);
		result = check_file(dirs[i]);
		if (result == 0 || result == 126)
		{
			errno = 0;
			free(info->tokens[0]);
			info->tokens[0] = _strdup(dirs[i]);
			free_ptrs(dirs);
			return (result);
		}
	}
	free(info->tokens[0]);
	info->tokens[0] = NULL;
	free_ptrs(dirs);
	return (result);
}

/**
 * get_path - tokenize path in dir
 * @info: commands passed
 * Return: array of path dir
 */

char **get_path(info_t *info)
{
	int i = 0;
	int count_dirs = 2;
	char **tokens = NULL;
	char *PATH;

	PATH = _getenv("PATH", info);
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
 * @full_path: ptr to full file name
 * Return: 0 success, error code on fail
 */

int check_file(char *full_path)
{
	struct stat sb;

	if (stat(full_path, &sb) != -1)
	{
		if (S_ISDIR(sb.st_mode) ||  access(full_path, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}
	errno = 127;
	return (127);
}
