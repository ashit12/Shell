#include "headers.h"
void echo(char str[])
{
	char *token = strtok(0, "\t ");
	while (token != NULL)
	{
		printf("%s ", token);
		token = strtok(0, "\t ");
	}
}
int cd(char *token, char homeDirectory[])
{
	char tempDirectory[PATH_MAX], buf[PATH_MAX];
	int i, s = 0;
	if (strcmp(token, "~") == 0 || strcmp(token, "~/") == 0)
	{
		chdir(homeDirectory);
		return 0;
	}
	if (token[0] == '/')
	{
		int ret = chdir(token);
		if (ret)
			return ret;
		if (strlen(token) < strlen(homeDirectory))
			return 1;
		else
			return 0;
	}
	int len = strlen(token);
	if (token[0] == '~')
	{
		strcpy(tempDirectory, homeDirectory);
		strcat(tempDirectory, token + 1);
	}
	else
		strcpy(tempDirectory, token);
	int ret = chdir(tempDirectory);
	if (ret)
		return ret;
	getcwd(buf, 1000);
	i = 0;
	int n = strlen(homeDirectory);
	while (buf[i] == homeDirectory[i])
		i++;
	if (i >= n)
		return 0;
	return 1;
}