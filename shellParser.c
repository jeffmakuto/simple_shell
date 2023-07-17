#include "shell.h"

/**
 * sigintHandler - Signal handler for SIGINT (Ctrl+C)
 *
 * @signum: Signal number
 *
 * This function handles the SIGINT signal (Ctrl+C) by printing a newline and
 * the prompt on the standard output.
 *
 * Return: Void
 */
void sigintHandler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n$ ", 3);
}

/**
 * handleIORedirection - Handle input/output redirection for command arguments
 *
 * @args: Array of command arguments
 *
 * This function checks the arguments array for '<', '>', and '>>' symbols,
 * indicating input/output redirection, and handles them accordingly by
 * modifying the file descriptors.
 *
 * Return: Void
 */
void handleIORedirection(char **args)
{
	int i = 0, fd;

	while (args[i])
	{
		if (strcmp(args[i], ">") == 0)
		{
			fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fd == -1)
			{
				perror("./hsh: open error");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}
		else if (strcmp(args[i], ">>") == 0)
		{
			fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fd == -1)
			{
				perror("./hsh: open error");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}
		else if (strcmp(args[i], "<") == 0)
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
			{
				perror("./hsh: open error");
				return;
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
		}
		i++;
	}
}

/**
 * executePipedCommands - Execute piped commands
 *
 * @cmds: String containing piped commands
 *
 * @envp: Environment variables
 *
 * This function executes a series of piped commands by splitting them
 * using the '|' symbol and then executing each command using the
 * executeSingleCommand function. It handles I/O redirection for each
 * individual command and sets up pipes to connect the commands.
 *
 * Return: Void
 */
void executePipedCommands(char *cmds, char **envp)
{
	char *cmd, **args, *end, *cmds_copy = strdup(cmds), *cmd_with_redirection;
	int i, pipefd[2], inputfd = STDIN_FILENO;
	pid_t pid;

	while ((cmd = strsep(&cmds_copy, "|")))
	{
		/* Trim leading and trailing whitespaces */
		while (*cmd && (*cmd == ' ' || *cmd == '\t'))
			cmd++;
		end = cmd + strlen(cmd) - 1;
		while (end > cmd && (*end == ' ' || *end == '\t' || *end == '\n'))
			end--;
		*(end + 1) = '\0';
		/* Handle I/O redirection for the current command */
		cmd_with_redirection = strdup(cmd);
		handleIORedirection(processCommand(cmd_with_redirection));
		free(cmd_with_redirection);
		/* Create a pipe if there are more commands to execute */
		if (cmds_copy)
		{
			if (pipe(pipefd) == -1)
			{
				perror("./hsh: pipe error");
				return;
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("./hsh: fork error");
			return;
		}
		if (pid == 0)
		{
			/* Child process */
			if (inputfd != STDIN_FILENO)
			{
				dup2(inputfd, STDIN_FILENO);
				close(inputfd);
			}
			if (cmds_copy)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			args = processCommand(cmd); /* Process the current command */
			if (args)
			{
				executeCommand(args, envp); /* Execute the command */
				for (i = 0; args[i]; i++)
					free(args[i]);
				free(args);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			/* Parent process */
			if (inputfd != STDIN_FILENO)
				close(inputfd);
			if (cmds_copy)
			{
				close(pipefd[1]);
				inputfd = pipefd[0];
			}
			waitpid(pid, NULL, 0);
		}
	}
	free(cmds_copy);
}

