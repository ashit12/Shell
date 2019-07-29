#include "headers.h"

void pinfo(char homeDirectory[], char str[])
{
	char *qwe = strtok(0, "\t\n ");
	char token[100];
	if (qwe != NULL)
	{
		int len = strlen(qwe);
		for (int i = 0; i < len; ++i)
			token[i] = qwe[i];
		token[len] = '\0';
	}
	else
		sprintf(token, "%d", getpid());
	char a1[100], a2[100], a3[100], mem[100], st[100], buf[PATH_MAX];
	strcpy(a1, "/proc/");
	strcpy(a2, "/proc/");
	strcpy(a3, "/proc/");
	strcat(a1, token);
	strcat(a2, token);
	strcat(a3, token);
	strcat(a1, "/status");
	strcat(a2, "/statm");
	strcat(a3, "/exe");
	FILE *pt1, *pt2;
	pt1 = fopen(a1, "r");
	pt2 = fopen(a2, "r");
	if (pt1 == NULL || pt2 == NULL)
	{
		printf("No process with given PID exists\n");
		return;
	}
	int ret = readlink(a3, buf, PATH_MAX);
	buf[ret] = '\0';
	fscanf(pt2, "%s", mem);
	int i = 0;
	while (i < 3)
		fgets(st, 100, pt1), i++;
	printf("pid -- %s\n", token);
	printf("Process %s", st);
	printf("%s {Virtual Memory}\n", mem);
	int n = strlen(homeDirectory);
	for (i = 0; i < n; i++)
		if (buf[i] != homeDirectory[i])
			break;
	if (i < n)
		printf("Executable path of file: %s", buf);
	else
	{
		printf("Executable path of file: ~");
		for (; buf[i] != '\0'; i++)
			printf("%c", buf[i]);
	}
	fclose(pt1);
	fclose(pt2);
}