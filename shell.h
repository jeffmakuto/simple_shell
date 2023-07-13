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

/* Define macros */
#define PROMPT "\n$ "
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
	void (*action)(char **args);
} BuiltinCmd;

/* Prototype functions */
void runInteractiveMode(char **envp);
void runNonInteractiveMode(char **envp);
int processCommandInput(char *cmd, char **envp);
char **processCommand(char *cmd);
void executeCommand(char **args, char **envp);
int checkBuiltins(char *cmd, char **args);
bool isAbsolutePath(const char *path);
int isExecutable(const char *path);
char *findExecutable(const char *cmd);
int processCommandLineArguments(int ac, char **av, char **envp);
void cdAction(char **args);
int changeDirectory(const char *targetDir);
void exitAction(char **args);

#endif /* SHELL_H */
