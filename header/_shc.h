#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

#include"etc/opt.h"
#include"etc/error.h"
#include"header/processcmd.h"

FILE *in;
static FILE *input[MAX_INPUT_FILE];
static char cmd[CMDLINE_MAX];