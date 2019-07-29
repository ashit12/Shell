#include "headers.h"
void ls(char *token)
{
	char *prev, currentDirectory[PATH_MAX];
	getcwd(currentDirectory, 1000);
	int counter = 0, flag;
	token = strtok(0, "\t\n ");
	int arr[2] = {0, 0};
	while (token != NULL)
	{
		prev = token;
		if (strncmp(prev, "-a", 2) == 0)
			arr[0] = 1;
		if (strncmp(prev, "-l", 2) == 0)
			arr[1] = 1;
		if (strncmp(prev, "-la", 3) == 0 || strncmp(prev, "-al", 3) == 0)
			arr[0] = arr[1] = 1;
		if (token != NULL && token[0] != '-')
		{
			if (token[0] == '~')
				strcat(currentDirectory, token + 1);
			else
				strcpy(currentDirectory, token);
		}
		token = strtok(0, "\t\n ");
	}
	if (arr[0] == 0 && arr[1] == 0)
		flag = 0;
	else if (arr[0] == 0 && arr[1] == 1)
		flag = 1;
	else if (arr[0] == 1 && arr[1] == 0)
		flag = 2;
	else if (arr[0] == 1 && arr[1] == 1)
		flag = 3;
	struct dirent *ent;
	DIR *dir;
	dir = opendir(currentDirectory);
	if (dir == NULL)
	{
		printf("No such file or directory could be found");
		return;
	}
	if (flag % 2 == 0)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_name[0] == '.' && flag == 0)
				continue;
			printf("%s ", ent->d_name);
		}
	}
	else
	{
		int n = strlen(currentDirectory);
		struct stat buf;
		struct passwd *pw;
		struct group *gr;
		char star[10], temp[200], date[200], type;
		while ((ent = readdir(dir)) != NULL)
		{
			//Finding the actual path of the file
			int i = 0, j;
			if (ent->d_name[0] == '.' && flag == 1)
				continue;
			for (i = 0; i < n; i++)
				temp[i] = currentDirectory[i];
			temp[n] = '/';
			int len = strlen(ent->d_name);
			for (j = 0; j < len; j++)
				temp[i + 1 + j] = ent->d_name[j];
			temp[i + 1 + j] = '\0';

			lstat(temp, &buf);
			if (S_ISREG(buf.st_mode))
				type = '-';
			else if (S_ISDIR(buf.st_mode))
				type = 'd';
			else if (S_ISLNK(buf.st_mode))
				type = 'l';
			mode_t ch = buf.st_mode;
			star[0] = (ch & S_IRUSR) ? 'r' : '-';
			star[1] = (ch & S_IWUSR) ? 'w' : '-';
			star[2] = (ch & S_IXUSR) ? 'x' : '-';
			star[6] = (ch & S_IROTH) ? 'r' : '-';
			star[7] = (ch & S_IWOTH) ? 'w' : '-';
			star[8] = (ch & S_IXOTH) ? 'x' : '-';
			star[3] = (ch & S_IRGRP) ? 'r' : '-';
			star[4] = (ch & S_IWGRP) ? 'w' : '-';
			star[5] = (ch & S_IXGRP) ? 'x' : '-';
			star[9] = '\0';
			pw = getpwuid(buf.st_uid);
			gr = getgrgid(buf.st_gid);
			strftime(date, 200, "%b %d %H:%M", localtime(&buf.st_mtime));
			printf("%c%-10s%ld  %-10s%-10s%8ld   %-14s%-10s\n", type, star, buf.st_nlink, pw->pw_name, gr->gr_name, buf.st_size, date, ent->d_name);
		}
	}
}