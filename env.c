#include "headers.h"
void set(char *token)
{
	char *tok = strtok(0, "\n\t ");
	int z, c = 0, f = 0;
	char name[100], value[100];
	char str[] = "";
	while (tok != NULL)
	{
		if (f == 1)
		{
			for (z = 0; tok[z] != '\0'; z++)
				value[z] = tok[z];
			value[z] = '\0';
			f = 2;
		}
		if (f == 0)
		{
			for (z = 0; tok[z] != '\0'; z++)
				name[z] = tok[z];
			name[z] = '\0';
			f = 1;
		}
		tok = strtok(0, "\n\t ");
		c++;
	}
	if (c == 1)
		value[0] = '\t', value[1] = '\0';
	if (c < 1 || c > 2)
	{
		printf("Invalid command");
		return;
	}
	else
		setenv(name, value, 1);
}
void unset(char *token)
{
	char *tok = strtok(0, "\n\t ");
	if (tok == NULL)
	{
		printf("Need to specify environment variable");
		return;
	}
	int z, c = 0;
	char name[100];
	while (tok != NULL)
	{
		for (z = 0; tok[z] != '\0'; z++)
			name[z] = tok[z];
		name[z] = '\0';
		tok = strtok(0, "\n\t ");
		c++;
	}
	if (c != 1)
	{
		printf("More than one variables specified");
		return;
	}
	else
		unsetenv(name);
}