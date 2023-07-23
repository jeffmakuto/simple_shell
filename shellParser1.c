#include "shell.h"

int lastExitStatus = 0;
/**
 * replaceVariables - Replace variables in a string
 *
 * @input: The input string
 *
 * Return: A new string with variables replaced.
 * Note: The returned string should be freed by the caller.
 */
char *replaceVariables(char *input)
{
    char *replacedStr = strdup(input);
    char *ptr = replacedStr;
    char pidStr[20]; // Buffer to hold the process ID string

    // Replace $? with the exit status of the last command
    if (strstr(replacedStr, "$?"))
    {
        snprintf(pidStr, sizeof(pidStr), "%d", lastExitStatus);
        ptr = strstr(replacedStr, "$?");
        *ptr = '\0';
        strcat(replacedStr, pidStr);
        strcat(replacedStr, ptr + 2);
    }

    // Replace $$ with the shell's process ID
    if (strstr(replacedStr, "$$"))
    {
        snprintf(pidStr, sizeof(pidStr), "%d", getpid());
        ptr = strstr(replacedStr, "$$");
        *ptr = '\0';
        strcat(replacedStr, pidStr);
        strcat(replacedStr, ptr + 2);
    }

    // Handle $PATH
    if (strstr(replacedStr, "$PATH"))
    {
        char *pathEnv = getenv("PATH");
        if (pathEnv)
        {
            ptr = strstr(replacedStr, "$PATH");
            *ptr = '\0';
            strcat(replacedStr, pathEnv);
            strcat(replacedStr, ptr + 5);
        }
    }

    return replacedStr;
}

