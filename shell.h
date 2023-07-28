#ifndef SHELL_H
#define SHELL_H

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

/* Define macros */
#define PROMPT "\n($) "
#define INITIAL_ENVP_SIZE 10
#define MAX_PATH_LEN 100
#define BUFFER_SIZE 1024

/* Struct definitions */

/**
 * struct prog_args - Represents the arguments and information for a
 * builtin command.
 *
 * @progName: The name of the program or command.
 *
 * @buffer: The buffer to store the input command line.
 *
 * @cmd: The command string.
 *
 * @count: The number of times the command has been executed.
 *
 * @fd: The file descriptor used for I/O redirection.
 *
 * @tokens: An array of strings representing the tokenized command arguments.
 *
 * @envp: An array of strings representing the environment variables.
 *
 * This struct holds various information related to a builtin command.
 * It is used to pass and store command-specific arguments and data.
 */
typedef struct prog_args
{
	char *prog_name;
	char *buffer;
	char *cmd;
	int count;
	int fd;
	char **tokens;
	char **envp;
	char **alias_list;
} prog_args;

/**
 * struct builtin_cmd - Represents a built-in command.
 *
 * @command: The command string.
 *
 * @action: A pointer to the function that implements the command's action.
 *
 * The builtin_cmd struct defines a built-in command and its associated action.
 */
typedef struct builtin_cmd
{
	char *command;
	int (*action)(prog_args *args);
} builtin_cmd;

/* Prototype functions */
void start_shell(prog_args *args, int ac, char *av[], char *envp[]);
void run_shell(char *prompt, prog_args *args);
void handle_ctrl_c_signal(int signal);
int find_executable(prog_args *args);
int check_file(char *file_path);
char **get_path(prog_args *args);
int execute_command(prog_args *args);
void process_command(prog_args *args);
void replace_variables(prog_args *args);
void free_args(prog_args *args);
void free_ptrs(char **arr);
void cleanup_after_execution(prog_args *args);
int handle_logical_operators(char *commands[], int i, char operators[]);
int check_builtins(prog_args *args);
int cd_action(prog_args *args);
int change_directory(prog_args *args, char *new_dir);
int env_action(prog_args *args);
int setenv_action(prog_args *args);
int unsetenv_action(prog_args *args);
int exit_action(prog_args *args);
void expand_alias(prog_args *args);
int alias_action(prog_args *args);
int print_alias(prog_args *args, char *alias);
char *get_alias(prog_args *args, char *alias);
int set_alias(char *alias_str, prog_args *args);

/*auxilliaries*/
int _strlen(char *str);
int _strncmp(char *str1, char *str2, int n);
int _atoi(char *str);
char *_strdup(char *str);
char *_strtok(char *line, char *delim);
char *_strcat(char *dest, char *src);
char *_getenv(char *name, prog_args *args);
int _setenv(char *name, char *value, prog_args *args);
int _unsetenv(char *name, prog_args *args);
void _printenv(prog_args *args);
int _getline(prog_args *args);
void long_to_str(long num, char *str, int base);
int buffcat(char *buffer, char *add_str);
void rev_str(char *str);
int _print(char *str);
int _printe(char *str);
int print_err(int err, prog_args *args);
int count_chars(char *str, char *chars);

#endif /* SHELL_H */
