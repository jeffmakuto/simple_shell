#include "shell.h"


/**
 * findExecutable - look for a program in path
 *
 * @args: Ptr to program data
 *
 * Return: 0 success, error code on fail
 */

int findExecutable(PROGARGS *args)
{
	int i = 0, result = 0;
	char **dirs;

	if (!args->cmd)
		return (-1);
	if (args->cmd[0] == '/' || args->cmd[0] == '.')
		return (checkFile(args->cmd));
	free(args->tokens[0]);
	args->tokens[0] = _strcat(_strdup("/"), args->cmd);
	if (!args->tokens[0])
		return (-1);
	dirs = tokenizePath(args);
	if (!dirs || !dirs[0])
	{
		perror("./hsh: Dir Errror");
		return (-1);
	}
	for (i = 0; dirs[i]; i++)
	{
		dirs[i] = _strcat(dirs[i], args->tokens[0]);
		result = checkFile(dirs[i]);
		 /* If file's found and executable, update and return 0 (success).*/
		if (!result)
		{
			free(args->tokens[0]);
			args->tokens[0] = _strdup(dirs[i]);
			freePtrs(dirs);
			return (0);
		}
		/* If file's non-executable, print error  message and return -1 (failure). */
		else if (result == -1 && errno == EACCES)
		{
			perror("./hsh: Permission denied");
			free(args->tokens[0]);
			args->tokens[0] = NULL;
			freePtrs(dirs);
			return (-1);
		}
	}
	free(args->tokens[0]);
	args->tokens[0] = NULL;
	freePtrs(dirs);
	return (result);
}

/**
 * getPath - Tokenize path in dirs
 *
 * @args: Ptr to program data
 *
 * Return: An array of path dirs
 */

char **getPath(PROGARGS *args)
{
	int i = 0;
	int countDirs = 2;
	char **tokens = NULL;
	char *PATH;

	PATH = _getenv("PATH", args);
	if ((PATH) == 0 || PATH[0] == '\0')
		return (NULL);

	PATH = _strdup(PATH);

	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			countDirs++;
	}

	tokens = malloc(sizeof(char *) * countDirs);

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
 * checkFile - Checks if a file exists, it's not a dir and
 * has excecution permisions for permisions.
 *
 * @filePath: Ptr to full file name
 *
 * Return: 0 success, error code on fail
 */

int checkFile(char *filePath)
{
	struct stat fileStat;

	if (stat(filePath, &fileStat) != -1)
	{
		if (S_ISDIR(fileStat.st_mode) ||  access(filePath, X_OK))
		{
			perror("./hsh: Error: Not an executable file");
			return (-1);
		}
		return (0);
	}
	perror("./hsh: Error: File not found");
	return (-1);
}
