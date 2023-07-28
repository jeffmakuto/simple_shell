#include "shell.h"

/**
 * cleanup_after_execution - free required memory
 * @info: commands passed
 * Return: void
 */
void cleanup_after_execution(info_t *info)
{
	if (info->tokens)
		free_ptrs(info->tokens);
	if (info->buff)
		free(info->buff);
	if (info->cmd)
		free(info->cmd);

	info->buff = NULL;
	info->cmd = NULL;
	info->tokens = NULL;
}

/**
 * free_info - free all memory
 * @info: commands passed
 * Return: void
 */
void free_info(info_t *info)
{
	if (info->fd != 0)
	{
		if (close(info->fd))
			perror(info->prog_name);
	}
	cleanup_after_execution(info);
	free_ptrs(info->env);
	free_ptrs(info->alias_list);
}

/**
 * free_ptrs - free ptr memory and array
 * @arr: array of pointers
 * Return: void
 */
void free_ptrs(char **arr)
{
	int i;

	if (arr != NULL)
	{
		for (i = 0; arr[i]; i++)
			free(arr[i]);

		free(arr);
		arr = NULL;
	}
}
