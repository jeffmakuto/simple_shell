#include "shell.h"

/**
 * execute - this function executes a command.
 * @info: commands passed
 * Return: 0 on success, -1 on fail
 */
int execute(info_t *info)
{
	int result = 0, status;
	pid_t pid;

	result = builtins_list(info);
	if (result != -1)
		return (result);

	result = find_executable(info);
	if (result)
	{
		return (result);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror(info->cmd);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			result = execve(info->tokens[0], info->tokens, info->env);
			if (result == -1)
				perror(info->cmd), exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
				errno = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				errno = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
