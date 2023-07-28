#ifndef SHELL_H
#define SHELL_H

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Define macros */
#define PROMPT "\n($) "
#define MAX_ARGS 100

/* Struct definitions */

/**
 * struct builtin_cmd - Represents a built-in command.
 *
 * @cmd: The command string.
 *
 * @action: A pointer to the function that implements the command's action.
 *
 * The builtin_cmd struct defines a built-in command and its associated action.
 */
typedef struct builtin_cmd
{
	char *cmd;
	void (*action)(char **args, char **envp);
} builtin_cmd;

/* Prototype functions */
void run_interactive_mode(char **envp);
void run_non_interactive_mode(char **envp);
char **handle_semi_coloned_commands(char *cmd, int *num_commands);
int handle_and_operator(char *cmd, char **envp);
int handle_or_operator(char *cmd, char **envp);
int process_command_input(char *cmd, char **envp);
int process_single_command(char *cmd, char **envp, int *last_exit_status_ptr);
char *replace_variables(char *input, char **envp);
char *replace_path_variable(char *input, char **envp);
char *int_to_string(int num, char *str);
char *concat_strings(char *str1, char *str2);
char **process_command(char *cmd);
void execute_command(char **args, char **envp);
int check_builtins(char *cmd, char **args, char **envp);
bool is_absolutePath(char *path);
int is_executable(char *path);
char *find_executable(char *cmd, char **envp);
int process_commandLine_arguments(int ac, char **av, char **envp);
void cd_action(char **args, char **envp);
int change_directory(char *target_dir);
void setenv_action(char **args, char **envp);
void unsetenv_action(char **args, char **envp);

/*auxilliaries*/
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
int _atoi(char *s);
char *_strchr(char *s, char c);
char *_strdup(char *str);
char *_memcpy(char *dest, char *src, unsigned int n);
char *_strstr(char *haystack, char *needle);
char *_strtok(char *line, char *delim);
char *_strncpy(char *dest, char *src, int n);
char *_strcat(char *dest, char *src);
void *_realloc(void *ptr, size_t new_size);
char *_getenv(char *name, char **envp);
int _strncmp(const char *str1, const char *str2, size_t n);
size_t _strcspn(const char *str, const char *reject);
char *_strtok_r(char *str, const char *delimiters, char **save_ptr);
unsigned int _strspn(char *s, const char *accept);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
void free_args(char **args);

#endif /* SHELL_H */

