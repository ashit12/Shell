#include "headers.h"

void remindme(char str[])
{
	char *token = strtok(0, "\t\n ");
	for (int i = 0; token[i] != '\0'; i++)
	{
		if (token[i] > '9' || token[i] < '0')
		{
			printf("Please provide a number as argument");
			return;
		}
	}
	int num = atoi(token);
	token = strtok(0, "");
	pid_t pid = fork();
	if (pid == 0)
	{
		sleep(num);
		printf("\nReminder: %s\n", token);
		exit(0);
	}
}
