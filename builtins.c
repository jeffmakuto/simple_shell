#include "shell.h"

/**
 * cdAction - change directory to the specified path.
 *
 * @args: An array of command arguments where args[1]
 * contains the target directory path.
 *
 * This function changes the current working directory
 * to the specified path.
 *
 * If no argument is provided, it displays an error message.
 * If the argument is "~", it changes the directory to the
 * home directory.
 * If the argument is "-", it changes the directory to the
 * previous directory.
 * The function also updates the OLDPWD environment variable
 *and displays the new path.
 *
 * Return: Void
 */
void cdAction(char **args)
{
	char *targetDir, cwd[PATH_MAX];

	if (args[1] == NULL)
		return;
	targetDir = args[1];
	if (strcmp(targetDir, "~") == 0)
	{
		targetDir = getenv("HOME");
		if (!targetDir)
			return;
	}
	else if (strcmp(targetDir, "-") == 0)
	{
		targetDir = getenv("OLDPWD");
		if (!targetDir)
			return;
		write(STDOUT_FILENO, targetDir, strlen(targetDir));
		write(STDOUT_FILENO, "\n", 1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd error");
		return;
	}
	if (chdir(targetDir))
	{
		perror("cd error");
		return;
	}
	if (setenv("OLDPWD", cwd, 1))
		perror("cd error");
}
