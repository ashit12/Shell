#include "headers.h"
extern int cur_pid,checkz;
void otherprocess(char *token, int procArray[], int size)
{
	int i, j;
	char command[100][15];
	i = 0;
	int flag = 0, n;
	while (token != NULL)
	{
		n = strlen(token);
		for (j = 0; j < n; j++)
			command[i][j] = token[j];
		command[i][j] = '\0';
		if (strncmp(command[i], "&", 1) == 0)
			flag = 1;
		i++;
		token = strtok(0, "\n\t ");
	}
	command[i][0] = '\0';

	char *arr[15];
	for (i = 0, j = 0; command[i][0] != '\0'; i++)
	{
		if (command[i][0] == '&')
			continue;
		arr[j++] = command[i];
	}
	arr[j] = NULL;
	if (flag == 0)
	{
		pid_t pid = fork();
		cur_pid=pid;
		if (pid == -1)
			printf("Child process not created");
		if (pid == 0)
		{
			int z = execvp(arr[0], arr);
			printf("%s: command not found", arr[0]); // If command does not exist
			exit(0);
		}
		else
		{
			int status;
			while (waitpid(pid,&status,WUNTRACED)>0)
			{
				if(checkz==1)
				{
					kill(pid,SIGSTOP);
					procArray[size]=pid;
					break;
				}
			}
		}
	}
	else
	{
		pid_t pid = fork();
		procArray[size] = pid;
		if (pid == -1)
			printf("Child process not created");
		if (pid == 0)
		{
			signal(SIGCHLD,SIG_IGN);
			setpgid(getpid(),pid);
			execvp(arr[0], arr);
			printf("%s: command not found", arr[0]);
			exit(0);
		}
	}
}