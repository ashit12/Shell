#include "headers.h"
extern int cur_pid,k,checkz;
void fg(int procArray[],int size)
{
    int i;
    char *tok = strtok(0, "\n\t ");
    signal(SIGINT,handler_main);
    signal(SIGTSTP,handler_z);
    if (tok == NULL)
    {
        printf("No job id specified");
        return;
    }
    for (i = 0; tok[i] != '\0'; i++)
    {
        if (tok[i] < '0' || tok[i] > '9')
        {
            printf("Invalid job ID");
            return;
        }
    }
    int pid = atoi(tok);
    char arr[100], st[100], pi[100];
    FILE *pt;
    int count = 0, stat;
    for (i = 0; i < 1000; i++)
    {
        if (procArray[i] > 0)
        {
            count++;
            if (count == pid)
            {
                int val = procArray[i];
                procArray[i] = -1;
                sprintf(pi, "/proc/%d/cmdline", val);
                pt = fopen(pi, "r");
                fgets(st, 100, pt);
                printf("%s", st);
                fflush(stdout);
                kill(val,SIGCONT);
                while(waitpid(val, &stat, WNOHANG)!=val)
                {
                    if(k==1)
                    {
                        kill(val,SIGINT);
                        k=0;
                        break;
                    }
                    if(checkz==1)
                    {
                        kill(val,SIGSTOP);
                        procArray[size]=val;
                        checkz=0;
                        break;
                    }
                }
                fclose(pt);
                return;
            }
        }
    }
    printf("No process with given job ID exists");
    return;
}
void bg(int procArray[])
{
    int i;
    char *tok = strtok(0, "\n\t ");
    if (tok == NULL)
    {
        printf("No job id specified");
        return;
    }
    for (i = 0; tok[i] != '\0'; i++)
    {
        if (tok[i] < '0' || tok[i] > '9')
        {
            printf("Invalid job ID");
            return;
        }
    }
    int pid = atoi(tok);
    int count = 0, stat;
    for (i = 0; i < 200; i++)
    {
        if (procArray[i] > 0)
        {
            count++;
            if (count == pid)
            {
                kill(procArray[i], SIGCONT);
                return;
            }
        }
    }
}