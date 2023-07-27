#ifndef SHELL_H
#define SHELL_H

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>

/* Define macros */
#define PROMPT "\n($) "
#define INITIAL_ENVP_SIZE 10

/* Struct definitions */

/**
 * struct PROGARGS - Represents the arguments and information for a
 * builtin command.
 *
 * @programName: The name of the program or command.
 *
 * @buffer: The buffer to store the input command line.
 *
 * @cmd: The command string.
 *
 * @execCount: The number of times the command has been executed.
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
	char *programName;
	char *buffer;
	char *cmd;
	int execCount;
	int fd;
	char **tokens;
	char **envp;
} PROGARGS;

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
	char *command;
	int (*action)(PROGARGS *data);
} BuiltinCmd;

/* Prototype functions */
void startShell(PROGARGS *args, int ac, char *av[], char *envp[]);
void runShell(char *prompt, PROGARGS *args);
void handleCtrlCSignal(int signal);
int checkFile(char *filePath);
char **getPath(PROGARGS *args);
char *findExecutable(PROGARGS *args);
int executeCommand(PROGARGS *args, int *exitStatus, int *termSig);
void splitCommands(PROGARGS *args);
int processCommandLineArguments(int ac, char **av, char **envp);


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

#endif /* SHELL_H */
