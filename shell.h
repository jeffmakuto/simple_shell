#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include "macros.h"

/* struct definitions */

/**
 * struct info- struct for the program's data
 * @prog_name: the name of the executable
 * @buff: pointer to the input read for _getline
 * @cmd: pointer to the first command typed by the user
 * @count: number of excecuted comands
 * @fd: file descriptor to the input of commands
 * @tokens: pointer to array of tokenized input
 * @env: copy of the environ
 * @alias_list: array of pointers with aliases.
 */
typedef struct info
{
	char *prog_name;
	char *buff;
	char *cmd;
	int count;
	int fd;
	char **tokens;
	char **env;
	char **alias_list;
} info_t;

/**
 * struct builtins - struct for the builtins
 * @command: the name of the builtin
 * @action: the associated function to be called for each builtin
 */
typedef struct builtins
{
	char *command;
	int (*action)(info_t *info);
} builtins;

/* prototypes */
void start_shell(info_t *info, int ac, char *av[], char **env);
void run_shell(char *prompt, info_t *info);
void handle_ctrl_c(int signal);
int _getline(info_t *info);
int handle_logical_operators(char *commands[], int i, char operators[]);
void expand_variables(info_t *info);
int buffer_cat(char *buffer, char *str_to_add);
void process_command(info_t *info);
char *_strtok(char *line, char *delim);
int execute(info_t *info);
int builtins_list(info_t *info);
char **get_path(info_t *info);
int find_executable(info_t *info);
void free_ptrs(char **dirs);
void cleanup_after_execution(info_t *info);
void free_info(info_t *info);
int exit_action(info_t *info);
int cd_action(info_t *info);
int change_dir(info_t *info, char *new_dir);
int env_action(info_t *info);
int setenv_action(info_t *info);
int unsetenv_action(info_t *info);
char *_getenv(char *name, info_t *info);
int _setenv(char *name, char *value, info_t *info);
int _unsetenv(char *name, info_t *info);
void _printenv(info_t *info);
int _print(char *str);
int _printe(char *str);
int _print_error(int err, info_t *info);
int _strlen(char *str);
char *_strdup(char *str);
int _strncmp(char *str1, char *str2, int num);
char *_strcat(char *str1, char *str2);
void rev_str(char *str);
void long_to_string(long num, char *str, int base);
int _atoi(char *str);

#endif /* SHELL_H */
