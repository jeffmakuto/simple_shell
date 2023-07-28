#ifndef SHELL_H
#define SHELL_H

#include <stdio.h> /* for printf*/
#include <unistd.h> /* for fork, execve*/
#include <stdlib.h>
#include <string.h> /* for strtok*/
#include <stddef.h>
#include <errno.h> /* for errno and perror */
#include <sys/types.h> /* for type pid */
#include <sys/wait.h> /* for wait */
#include <sys/stat.h> /* for use of stat function */
#include <signal.h> /* for signal management */
#include <fcntl.h> /* for open files*/

/************* MACROS **************/

#include "macros.h" /* for msg help and prompt */

/************* STRUCTURES **************/

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
typedef struct info_t
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
 * @builtin: the name of the builtin
 * @action: the associated function to be called for each builtin
 */
typedef struct builtins
{
	char *command;
	int (*action)(info_t *info);
} builtins;


/************* MAIN FUNCTIONS *************/


/*========  shell.c  ========*/

/* Inicialize the struct with the info of the program */
void start_shell(info_t *info, int ac, char *av[], char **env);

/* Makes the infinite loop that shows the prompt*/
void run_shell(char *prompt, info_t *info);

/* Print the prompt in a new line */
void handle_ctrl_c(int signal);


/*========  _getline.c  ========*/

/* Read one line of the standar input*/
int _getline(info_t *info);

/* split the each line for the logical operators if it exist */
int handle_logical_operators(char *commands[], int i, char operators[]);


/*======== expansions.c ========*/

/* expand variables */
void expand_variables(info_t *info);

/* expand aliases */
void expand_alias(info_t *info);

/* append the string to the end of the buffer*/
int buffer_cat(char *buffer, char *str_to_add);


/*======== str_tok.c ========*/

/* Separate the string in tokens using a designed delimiter */
void process_command(info_t *info);

/* Creates a pointer to a part of a string */
char *_strtok(char *line, char *delim);


/*======== execute.c ========*/

/* Execute a command with its entire path */
int execute(info_t *info);


/*======== builtins_list.c ========*/

/* If match a builtin, executes it */
int builtins_list(info_t *info);


/*======== find_in_path.c ========*/

/* Creates an array of the path directories */
char **get_path(info_t *info);

/* Search for program in path */
int find_executable(info_t *info);


/************** HELPERS FOR MEMORY MANAGEMENT **************/


/*======== helpers_free.c ========*/

/* Frees the memory for directories */
void free_ptrs(char **dirs);

/* Free the fields needed each loop */
void cleanup_after_execution(info_t *info);

/* Free all field of the data */
void free_info(info_t *info);


/************** BUILTINS **************/


/*======== builtins_more.c ========*/

/* Close the shell */
int exit_action(info_t *info);

/* Change the current directory */
int cd_action(info_t *info);

/* set the work directory */
int change_dir(info_t *info, char *new_dir);

/* show help information */
int help_action(info_t *info);

/* set, unset and show alias */
int alias_action(info_t *info);


/*======== builtins_env.c ========*/

/* Shows the environment where the shell runs */
int env_action(info_t *info);

/* create or override a variable of environment */
int setenv_action(info_t *info);

/* delete a variable of environment */
int unsetenv_action(info_t *info);


/************** HELPERS FOR ENVIRONMENT VARIABLES MANAGEMENT **************/


/*======== env_management.c ========*/

/* Gets the value of an environment variable */
char *_getenv(char *name, info_t *info);

/* Overwrite the value of the environment variable */
int _setenv(char *name, char *value, info_t *info);

/* Remove a key from the environment */
int _unsetenv(char *name, info_t *info);

/* prints the current environ */
void _printenv(info_t *info);


/************** HELPERS FOR PRINTING **************/


/*======== helpers_print.c ========*/

/* Prints a string in the standar output */
int _print(char *str);

/* Prints a string in the standar error */
int _printe(char *str);

/* Prints a string in the standar error */
int _print_error(int err, info_t *info);


/************** HELPERS FOR STRINGS MANAGEMENT **************/


/*======== helpers_string.c ========*/

/* Counts the number of characters of a string */
int _strlen(char *str);

/* Duplicates an string */
char *_strdup(char *str);

/* Compares two strings */
int _strncmp(char *str1, char *str2, int num);

/* Concatenates two strings */
char *_strcat(char *str1, char *str2);

/* Reverse a string */
void rev_str(char *str);


/*======== helpers_numbers.c ========*/

/* Cast from int to string */
void long_to_string(long num, char *str, int base);

/* convert an string in to a number */
int _atoi(char *str);

/* count the coincidences of character in string */
int count_chars(char *str, char *chars);


/*======== alias_management.c ========*/

/* print the list of alias */
int print_alias(info_t *info, char *alias);

/* get the alias name */
char *get_alias(info_t *info, char *alias);

/* set the alias name */
int set_alias(char *alias_str, info_t *info);


#endif /* SHELL_H */
