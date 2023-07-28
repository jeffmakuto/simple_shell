#include "shell.h"

/**
 * process_command - processes an array of string commands
 *
 * @cmd: the commands to be processed
 *
 * Return: array of string commands
 */
char **process_command(char *cmd)
{
	char **args = malloc(sizeof(char *) * MAX_ARGS), *token;
	int arg_count = 0;

	if (!args)
	{
		perror("./hsh: malloc error");
		return (NULL);
	}
	/* Ignore anything after the '#' character (comment handling) */
	token = _strtok(cmd, "#");
	if (!token)
	{
		free(args);
		return (NULL);
	}
	token = _strtok(cmd, " ");
	while (token && arg_count < MAX_ARGS)
	{
		args[arg_count] = _strdup(token);

		if (!args[arg_count])
		{
			perror("./hsh: strdup error");
			/* Free previously allocated strings */
			free_args(args);
			return (NULL);
		}
		arg_count++;

		if (_strchr(token, '\n'))
		{
			args[arg_count] = NULL;
			break; /* Exit the loop after setting NULL terminator */
		}
		token = _strtok(NULL, " ");
	}
	free(token);

	args[arg_count] = NULL;
	return (args);
}

/**
 * execute_command - execute string of arguments
 *
 * @args: string of arguments to be executed
 *
 * @envp: environment variable arguments
 *
 * Return: Void
 */
void execute_command(char **args, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("./hsh: fork error");
		return;
	}
	if (pid == 0)
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror("./hsh: execve error");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
}

/**
 * is_absolute_path - Checks if the given path is an absolute path.
 *
 * @path: The path to check.
 *
 * Return: true if the path is an absolute path and executable,
 * false otherwise.
 */
bool is_absolute_path(char *path)
{
	if (path[0] == '/')
	{
		if (is_executable(path))
			return (true);
		perror("./hsh: ");
		return (false);
	}
	return (false);
}

/**
 * is_executable - Checks if the given path is executable.
 *
 * @path: The path to check.
 *
 * Return: 1 if the path is executable, 0 otherwise.
 */
int is_executable(char *path)
{
	/* Check if the given path is executable */
	return (access(path, X_OK) == 0);
}

/**
 * find_executable - Searches for the executable in the system's
 * PATH environment variable.
 *
 * @cmd: The command to find.
 *
 * @envp: The environment variables
 *
 * Return: A pointer to the executable path if found, NULL otherwise.
 */
char *find_executable(char *cmd, char **envp)
{
	char *path = _getenv("PATH", envp), *path_env, *dir, *executable_path;
	size_t dir_len, cmd_len;
	char *found_executable = NULL;

	if (is_absolute_path(cmd))
		return (_strdup(cmd));
	if (!path)
	{
		perror("./hsh: PATH environment variable not found");
		return (NULL);
	}
	path_env = _strdup(path);
	dir = _strtok(path_env, ":");
	while (dir)
	{
		dir_len = _strlen(dir);
		cmd_len = _strlen(cmd);
		executable_path = malloc(dir_len + cmd_len + 2);
		if (!executable_path)
		{
			perror("./hsh: Memory allocation failed");
			free(path_env);
			return (NULL);
		}
		_memcpy(executable_path, dir, dir_len);
		executable_path[dir_len] = '/';
		_memcpy(executable_path + dir_len + 1, cmd, cmd_len + 1);
		if (is_executable(executable_path))
		{
			found_executable = executable_path;
			break;
		}
		free(executable_path);
		dir = _strtok(NULL, ":");
	}
	free(path_env);
	if (found_executable)
		return (found_executable);
	perror("./hsh: ");
	return (NULL);
}

