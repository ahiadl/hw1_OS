#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
typedef enum { FALSE , TRUE } bool;

int ExeCmd(Plist jobs, char* lineSize, char* cmdString);
int ExeComp(Plist jobs, char* lineSize);
int BgCmd(char* lineSize, Plist jobs);
void ExeExternal(char *args[MAX_ARG], char* cmdString,Plist jobs);
#endif

