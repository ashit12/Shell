#include "headers.h"
int st = 0;
void handler_clock(int val)
{
	if (val == SIGINT)
		st++;
}
void clocker(char str[])
{
	st = 0;
	signal(SIGINT, handler_clock);
	FILE *pt;
	char date[1000], time[1000];
	char *token = strtok(0, "\t\n ");
	for (int i = 0; token[i] != '\0'; i++)
	{
		if (token[i] > '9' || token[i] < '0')
		{
			printf("Please provide a number as an argument");
			return;
		}
	}
	int timer = atoi(token);
	pid_t pid = fork();
	if (pid == 0)
	{
		while (st == 0)
		{
			pt = fopen("/proc/driver/rtc", "r");
			fgets(time, 1000, pt);
			token = strtok(time, "\t\n ");
			token = strtok(0, "\t\n ");
			token = strtok(0, "\t\n ");
			printf("%s, ", token);
			fgets(date, 1000, pt);
			token = strtok(date, "\t\n ");
			token = strtok(0, "\t\n ");
			token = strtok(0, "\t\n ");
			printf("%s\n", token);
			sleep(timer);
		}
		exit(0);
	}
}