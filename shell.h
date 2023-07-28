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
 * struct PROGARGS - Represents the arguments and information for a
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
typedef struct PROGARGS
{
	char *progName;
	char *buffer;
	char *cmd;
	int count;
	int fd;
	char **tokens;
	char **envp;
	char **aliasList;
} PROGARGS;

/**
 * struct BuiltinCmd - Represents a built-in command.
 *
 * @command: The command string.
 *
 * @action: A pointer to the function that implements the command's action.
 *
 * The BuiltinCmd struct defines a built-in command and its associated action.
 */
typedef struct BuiltinCmd
{
	char *command;
	int (*action)(PROGARGS *args);
} BuiltinCmd;

/* Prototype functions */
void startShell(PROGARGS *args, int ac, char *av[], char *envp[]);
void runShell(char *prompt, PROGARGS *args);
void handleCtrlCSignal(int signal);
int findExecutable(PROGARGS *args);
int checkFile(char *filePath);
char **getPath(PROGARGS *args);
int executeCommand(PROGARGS *args);
void processCommand(PROGARGS *args);
void replaceVariables(PROGARGS *args);
void freeArgs(PROGARGS *args);
void freePtrs(char **arr);
void cleanupAfterExecution(PROGARGS *args);
int handleLogicalOperators(char *commands[], int i, char operators[]);
int checkBuiltins(PROGARGS *args);
int cdAction(PROGARGS *args);
int changeDirectory(PROGARGS *args, char *newDir);
int envAction(PROGARGS *args);
int setenvAction(PROGARGS *args);
int unsetenvAction(PROGARGS *args);
int exitAction(PROGARGS *args);
void expandAlias(PROGARGS *args);
int aliasAction(PROGARGS *args);
int printAlias(PROGARGS *args, char *alias);
char *getAlias(PROGARGS *args, char *alias);
int setAlias(char *aliasStr, PROGARGS *args);

/*auxilliaries*/
int _strlen(char *str);
int _strncmp(char *str1, char *str2, int n);
int _atoi(char *str);
char *_strdup(char *str);
char *_strtok(char *line, char *delim);
char *_strcat(char *dest, char *src);
char *_getenv(char *name, PROGARGS *args);
int _setenv(char *name, char *value, PROGARGS *args);
int _unsetenv(char *name, PROGARGS *args);
void _printenv(PROGARGS *args);
int _getline(PROGARGS *args);
void longToStr(long num, char *str, int base);
int buffcat(char *buffer, char *addStr);
void rev_str(char *str);
int _print(char *str);
int _printe(char *str);
int printErr(int err, PROGARGS *args);
int countChars(char *str, char *chars);
#endif /* SHELL_H */
