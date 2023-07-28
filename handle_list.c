#include "shell.h"

/**
 * builtins_list - search and execute builtins
 * @info: commands passed
 * Return: Return of the executed action, or -1.
 */
int builtins_list(info_t *info)
{
	int i;
	builtins options[] = {
		{"cd", cd_action},
		{"setenv", setenv_action},
		{"unsetenv", unsetenv_action},
		{"exit", exit_action},
		{"help", help_action},
		{"alias", alias_action},
		{"env", env_action},
		{NULL, NULL}
	};

	for (i = 0; options[i].command != NULL; i++)
	{
		if (_strncmp(options[i].command, info->cmd, 0))
		{
			return (options[i].action(info));
		}
	}
	return (-1);
}
