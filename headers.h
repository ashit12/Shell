#ifndef HEAD_H
#define HEAD_H
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <grp.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <signal.h>
void remindme(char str[]);
void otherprocess(char *token, int procArray[], int size);
void handler_clock(int sig_num);
void handler_main(int sig_num);
void handler_z(int sig_num);
void clocker(char str[]);
void echo(char str[]);
int cd(char *token, char homeDirectory[]);
void pinfo(char homeDirectory[], char str[]);
void ls(char *token);
void redirectandpipe(char *token, int flag, int pip);
void set(char *token);
void unset(char *token);
void jobs(int procArray[]);
void kjob(int procArray[], char *token);
void overkill(int procArray[]);
void fg(int procArray[],int size);
void bg(int procArray[]);
#endif