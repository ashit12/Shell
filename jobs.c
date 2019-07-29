#include "headers.h"
void jobs(int procArray[])
{
    int i, count = 1;
    char arr[1000], token[100], st[1000];
    char *t1 = strtok(0, "\n\t ");
    if (t1 != NULL)
    {
        printf("Jobs command does not take any argument");
        return;
    }
    for (i = 0; i < 200; i++)
    {
        if (procArray[i] > 0)
        {
            int val = procArray[i];
            printf("[%d]\t", count);
            strcpy(arr, "/proc/");
            sprintf(token, "%d", procArray[i]);
            strcat(arr, token);
            strcat(arr, "/status");
            FILE *pt;
            pt = fopen(arr, "r");
            int i = 0;
            while (i < 3)
                fgets(st, 100, pt), i++;
            fclose(pt);
            char *tok = strtok(st, "\n\t ");
            tok = strtok(0, "\n\t ");
            printf("%s", tok);
            tok = strtok(0, "\n\t ");
            printf("%s\t", tok);
            strcpy(arr, "/proc/");
            strcat(arr, token);
            strcat(arr, "/cmdline");
            pt = fopen(arr, "r");
            fgets(st, 100, pt);
            printf("%s", st);
            printf("[%d]\n", val);
            fclose(pt);
            count++;
        }
    }
}
void kjob(int procArray[], char *token)
{
    int count = 0;
    int flag = 0, jobid, sig, i;
    char *t1 = strtok(0, "\n\t ");
    if (t1 == NULL)
    {
        printf("Less number of arguments than expected");
        return;
    }
    for (i = 0; t1[i] != '\0'; i++)
        if (t1[i] < '0' || t1[i] > '9')
            break;
    if (t1[i] != '\0')
    {
        printf("Invalid PID");
        return;
    }
    else
        jobid = atoi(t1);
    t1 = strtok(0, "\n\t ");
    if (t1 == NULL)
    {
        printf("Less number of arguments than expected");
        return;
    }
    for (i = 0; t1[i] != '\0'; i++)
        if (t1[i] < '0' || t1[i] > '9')
            break;
    if (t1[i] != '\0')
    {
        printf("Invalid Signal");
        return;
    }
    else
        sig = atoi(t1);
    for (i = 0; i < 200; i++)
    {
        if (procArray[i] > 0)
        {
            count++;
            if (count == jobid)
            {
                flag = 1;
                kill(procArray[i], sig);
                return ;
            }
        }
    }
    if (!flag)
    {
        printf("No process with given job ID exists");
        return;
    }
}
void overkill(int procArray[])
{
    char *tok = strtok(0, "\n\t ");
    if (tok != NULL)
    {
        printf("Overkill does not take any argument");
        return;
    }
    for (int i = 0; i < 200; i++)
    {
        if (procArray[i] > 0)
        {
            kill(procArray[i], 9);
        }
    }
}