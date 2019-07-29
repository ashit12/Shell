#include "headers.h"

void redirectandpipe(char *token, int flag, int pip)
{
    /*
        flag=1 ->   <
        flag=2 ->   >
        flag=3 ->   >>
        flag=4 ->  <..>
        flag=5 ->  <..>>
    */
    int makepipe[200];
    int i, j, k, z, fd, val, check1, check2, prev = 0, p, gz = 0, f = 0, fdz;
    char temparray[1000], command[100][100], out[1000], inp[1000], exactcommand[1000], *cmd[100], tempcommand[100][100];
    if (pip)
    {
        for (i = 0; i < 200; i += 2)
            pipe(makepipe + i);
        // Copying each pipe command into one row of command array
        char *tok = strtok(token, "|");
        int j = 0, count = 0;
        while (tok != NULL)
        {
            // We scan the string and only take that portion which is after <
            if (flag == 1 || flag == 4 || flag == 5)
            {
                for (i = 0; tok[i] != '\0'; i++)
                    if (tok[i] == '<')
                        f = 1;
            }
            if (flag == 1 || flag == 4 || flag == 5)
            {
                if (f)
                    strcpy(command[j++], tok);
                else
                    pip--;
            }
            else
                strcpy(command[j++], tok);
            tok = strtok(0, "|");
        }
        for (i = 0, p = 0; i <= pip; i++, p += 2)
        {
            j = 0;
            // printf("%s",tok);
            // If there is < involved then we need to ignore <
            if (i == 0 && (flag == 1 || flag == 5 || flag == 4))
            {
                char *tok = strtok(command[i], "\n\t ");
                while (tok != NULL)
                {
                    if (strcmp(tok, "<") == 0)
                    {
                        tok = strtok(0, "\n\t ");
                        continue;
                    }
                    for (z = 0; tok[z] != '\0'; z++)
                        tempcommand[j][z] = tok[z];
                    tempcommand[j][z] = '\0';
                    strcpy(inp, tempcommand[j]);
                    tok = strtok(0, "\n\t ");
                    j++;
                }
            }
            else if (i == pip && flag > 1)
            {
                char *tok = strtok(command[i], "\n\t ");
                while (tok != NULL)
                {
                    if (strcmp(tok, ">") == 0 || strcmp(tok, ">>") == 0)
                    {
                        tok = strtok(0, "\n\t ");
                        continue;
                    }
                    for (z = 0; tok[z] != '\0'; z++)
                        tempcommand[j][z] = tok[z];
                    tempcommand[j][z] = '\0';
                    strcpy(out, tempcommand[j]);
                    tok = strtok(0, "\n\t ");
                    j++;
                }
            }
            else
            {
                char *tok = strtok(command[i], "\n\t ");
                while (tok != NULL)
                {
                    for (z = 0; tok[z] != '\0'; z++)
                        tempcommand[j][z] = tok[z];
                    tempcommand[j][z] = '\0';
                    tok = strtok(0, "\n\t ");
                    j++;
                }
            }

            for (k = 0; k < j; k++)
                cmd[k] = tempcommand[k];
            cmd[k] = NULL;

            // For the first command in the sequence
            if (i == 0)
            {
                int g = 0;
                int done = 0;
                if ((flag == 1 || flag == 4 || flag == 5) && done == 0)
                {
                    fd = open(cmd[k - 1], O_RDONLY);
                    if (fd == -1)
                    {
                        printf("No such file exists");
                        return;
                    }
                    g = done = 1;
                    cmd[k - 1] = NULL;
                }
                pid_t pid = fork();
                if (pid == 0)
                {
                    if (g)
                        dup2(fd, 0);
                    dup2(makepipe[1], 1);
                    execvp(cmd[0], cmd);
                    printf("%s: command does not exist", cmd[0]);
                    exit(0);
                }
                else
                {
                    wait(NULL);
                    if (g)
                        close(fd);
                    close(makepipe[1]);
                    g = 0;
                }
            }

            // For the last command in the sequence
            else if (i == pip)
            {
                int g = 0;
                if (flag > 1)
                {
                    if (flag % 2 == 0)
                        fd = open(cmd[k - 1], O_TRUNC | O_CREAT | O_WRONLY, 0644);
                    else
                        fd = open(cmd[k - 1], O_APPEND | O_CREAT | O_WRONLY, 0644);
                    g = 1;
                    cmd[k - 1] = NULL;
                }
                int pid = fork();
                if (pid == 0)
                {

                    if (g)
                        dup2(fd, 1);
                    dup2(makepipe[prev], 0);
                    execvp(cmd[0], cmd);
                    printf("%s: command does not exist", cmd[0]);
                    exit(0);
                }
                else
                {
                    wait(NULL);
                    if (g)
                        close(fd);
                    close(makepipe[prev]);
                }
            }

            // To execute all intermediate commands
            else
            {
                pid_t pid = fork();
                if (pid == 0)
                {
                    dup2(makepipe[prev], 0);
                    dup2(makepipe[p + 1], 1);
                    execvp(cmd[0], cmd);
                    printf("%s: command does not exist", cmd[0]);
                    exit(0);
                }
                else
                {
                    wait(NULL);
                    close(makepipe[prev]);
                    close(makepipe[p + 1]);
                    gz = 1;
                    prev = p;
                }
            }
        }
    }
    // If no piping is involved
    else
    {
        for (i = 0; token[i] != '\0'; i++)
        {
            if (token[i] == '<' && flag == 1)
                break;
            if (token[i] == '>' && flag > 1)
                break;
            temparray[i] = token[i];
        }
        temparray[i] = '\0';
        if (flag == 3 || flag == 5)
            i += 2;
        else
            i++;
        j = 0;
        for (; token[i] != '\0'; i++)
            out[j++] = token[i];
        out[j] = '\0';
        char *outdest = strtok(out, "\n\t ");
        char *arr[15];
        char *tok = strtok(temparray, "\n\t ");
        i = 0, j = 0;
        while (tok != NULL)
        {
            if (strcmp(tok, "<") == 0)
            {
                tok = strtok(0, "\n\t ");
                continue;
            }
            for (j = 0; tok[j] != '\0'; j++)
                command[i][j] = tok[j];
            command[i][j] = '\0';
            i++;
            tok = strtok(0, "\n\t ");
        }
        command[i][0] = '\0';
        for (i = 0, j = 0; command[i][0] != '\0'; i++)
            arr[j++] = command[i];

        if (flag == 1)
            arr[j++] = outdest;
        arr[j] = NULL;

        struct stat buff;
        if (flag == 1)
        {
            fd = open(arr[j-1],O_RDONLY);
            if (fd==-1)
            {
                printf("%s: No such file or directory", outdest);
                return;
            }
            else
            {
                arr[j-1]=NULL;
                pid_t pid = fork();
                if (pid == 0)
                {
                    dup2(fd,0);
                    execvp(arr[0], arr);
                    exit(0);
                }
                else
                {
                    int status;
                    while (wait(&status) != pid)
                        continue;
                    close(fd);
                }
            }
        }
        else
        {
            char *check = arr[j - 1];
            if (flag >= 4)
                val = stat(check, &buff);
            if (flag >= 4 && val)
            {
                printf("%s: No such file or directory", check);
                return;
            }
            else
                fdz = open(check,O_RDONLY);
            if (flag == 2 || flag == 4)
                fd = open(outdest, O_TRUNC | O_CREAT | O_WRONLY, 0644);
            else
                fd = open(outdest, O_CREAT | O_APPEND | O_WRONLY, 0644);
            pid_t pid = fork();
            if (pid == 0)
            {
                if(flag>3)
                    arr[j-1]=NULL;
                dup2(fd, 1);
                dup2(fdz,0);
                execvp(arr[0], arr);
                exit(0);
                printf("%s: could not be executed",arr[0]);
            }
            else
            {
                int status;
                while (wait(&status) != pid)
                    continue;
            }
            close(fd);
        }
    }
}