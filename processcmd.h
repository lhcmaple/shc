#ifndef _PROCESSCMD_H
#define _PROCESSCMD_H
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>

#include"opt.h"

int processcmd(char *cmd);

#endif