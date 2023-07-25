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
 * struct BuiltinCmd - Represents a built-in command.
 *
 * @cmd: The command string.
 *
 * @action: A pointer to the function that implements the command's action.
 *
 * The BuiltinCmd struct defines a built-in command and its associated action.
 */
typedef struct BuiltinCmd
{
	char *cmd;
	void (*action)(char **args, char **envp);
} BuiltinCmd;

/* Prototype functions */
void runInteractiveMode(char **envp);
void runNonInteractiveMode(char **envp);
char **handleSemiColonedCommands(char *cmd, int *numCommands);
int handleAndOperator(char *cmd, char **envp);
int handleOrOperator(char *cmd, char **envp);
int processCommandInput(char *cmd, char **envp);
int processSingleCommand(char *cmd, char **envp, int *lastExitStatusPtr);
char *replaceVariables(char *input, char **envp);
char *replacePathVariable(char *input, char **envp);
char *intToString(int num, char *str);
char *concatStrings(char *str1, char *str2);
char **processCommand(char *cmd);
void executeCommand(char **args, char **envp);
int checkBuiltins(char *cmd, char **args, char **envp);
bool isAbsolutePath(char *path);
int isExecutable(char *path);
char *findExecutable(char *cmd, char **envp);
int processCommandLineArguments(int ac, char **av, char **envp);
void cdAction(char **args, char **envp);
int changeDirectory(char *targetDir);
void setenvAction(char **args, char **envp);
void unsetenvAction(char **args, char **envp);

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

#endif /* SHELL_H */
