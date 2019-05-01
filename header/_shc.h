#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

#include"etc/opt.h"
#include"etc/error.h"
#include"header/processcmd.h"

char cmd[CMDLINE_MAX];
FILE *in;
static FILE *input[MAX_INPUT_FILE];