#include "headers.h"
// Check error handling

int cur_pid=-1,mypid,checkz,k;
void handler_main(int sig_num)
{
	k=1;	
}
void handler_z(int sig_num)
{
	checkz=1;
}
int main()
{
	signal(SIGINT,handler_main);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTSTP,handler_z);
	signal(SIGTERM,SIG_IGN);
	int i, n, j, procArray[2000], check = 0, size = 0,d =0;
	for (i = 0; i < 2000; i++)
		procArray[i] = 0;
	struct passwd *pws;
	struct utsname user;
	char homeDirectory[PATH_MAX], str[1000], currentDirectory[PATH_MAX], command[100][15], path[PATH_MAX], absPath[PATH_MAX], single[100][PATH_MAX];
	uname(&user);
	pws = getpwuid(geteuid());
	getcwd(homeDirectory, 1000);
	mypid = getpid();
	setpgid(mypid,mypid);
	while (1)
	{
		// To check if any program exited abnormally
		int stat, che, po = 0;
		checkz=0,k=0;
		che = waitpid(-1, &stat, WNOHANG);
		if (che > 0)
		{
			if (!WIFEXITED(stat))
			{
				printf("Process with pid = %d exited abnormally due to signal: %d\n", che, WTERMSIG(stat));
				for (i = 0; i < 1000; i++)
					if (procArray[i] == che)
						procArray[i] = -1;
				continue;
			}
		}

		// To check the exit status of any child background process
		for (i = 0; i < 1000; i++)
		{
			if (procArray[i] > 0 && kill(procArray[i], 0) == -1)
			{
				printf("Process with PID %d exited normally\n", procArray[i]);
				procArray[i] = -1;
				po = 1;
				break;
			}
		}
		if (po == 1)
			continue;
		// To print the username along with current directory
		getcwd(currentDirectory, 1000);
		i = 0, n = strlen(currentDirectory);
		while (i < n && currentDirectory[i] == homeDirectory[i])
			i++;
		if (check == 0)
		{
			printf("<%s@%s:~", pws->pw_name, user.nodename);
			while (i < n)
			{
				printf("%c", currentDirectory[i]);
				i++;
			}
		}
		else
		{
			getcwd(absPath, 1000);
			printf("<%s@%s:%s", pws->pw_name, user.nodename, absPath);
		}
		printf("> ");

		// To handle the commands inputted by the user
		fgets(str, 1000, stdin);
		int num = 0;
		char *breakcommand = strtok(str, "\n;");
		while (breakcommand != NULL)
		{
			strcpy(single[num], breakcommand);
			breakcommand = strtok(0, "\n;");
			num++;
		}
		int z = 0;
		char tempstring[1000];
		while (z < num)
		{
			strcpy(tempstring, single[z]);
			int len = strlen(tempstring), flag = 0, f = 0;
			for (i = 0; i < len; i++)
			{
				if (tempstring[i] == '|')
					f++;
				if (tempstring[i] == '<')
					flag = 1;
				if (tempstring[i] == '>')
				{
					if (flag == 1)
					{
						if (tempstring[i + 1] == '>')
							flag = 5, i++;
						else
							flag = 4;
					}
					else
					{
						if (tempstring[i + 1] == '>')
							flag = 3, i++;
						else
							flag = 2;
					}
				}
			}
			if (flag || f)
				redirectandpipe(tempstring, flag, f);
			else
			{
				char *token = strtok(tempstring, "\n\t ");
				if(token == NULL)
				{
					z++;
					break;
				}
				// echo command
				if (strcmp(token, "echo") == 0)
					echo(tempstring);

				// pwd command
				else if (strcmp(token, "pwd") == 0)
					printf("%s\n", currentDirectory);

				// cd command
				else if (strcmp(token, "cd") == 0)
				{
					token = strtok(0, "\t\n ");
					if (token != NULL)
					{
						int ret = cd(token, homeDirectory);
						if (ret == -1)
							printf("cd: %s: File or directory not found", token);
						else if (ret == 1)
							check = 1;
						else
							check = 0;
					}
					else
					{
						check = 0;
						chdir(homeDirectory);
					}
				}

				// ls command
				else if (strcmp(token, "ls") == 0)
					ls(token);

				// to exit the current terminal
				else if (strcmp(token, "quit") == 0)
					exit(0);

				// remindme command
				else if (strcmp(token, "remindme") == 0)
					remindme(tempstring);

				// clock command
				else if (strcmp(token, "clock") == 0)
					clocker(tempstring);

				//pinfo command
				else if (strcmp(token, "pinfo") == 0)
					pinfo(homeDirectory, tempstring);

				//setenv command
				else if (strcmp(token, "setenv") == 0)
					set(tempstring);

				//unsetenv command
				else if (strcmp(token, "unsetenv") == 0)
					unset(tempstring);

				//jobs command
				else if (strcmp(token, "jobs") == 0)
					jobs(procArray);

				//kjob commmand
				else if (strcmp(token, "kjob") == 0)
					kjob(procArray, tempstring);

				//overkill command
				else if (strcmp(token, "overkill") == 0)
					overkill(procArray);

				//fg command
				else if (strcmp(token,"fg")==0)
				{
					fg(procArray,size);
					size++;
				}
				//bg command
				else if(strcmp(token,"bg")==0)
					bg(procArray);

				// any other command apart from these
				else
				{
					otherprocess(token, procArray, size);
					size++;
				}
			}
			z++;
			printf("\n");
		}
	}
	return 0;
}