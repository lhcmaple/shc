#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>

#include"etc/opt.h"
#include"etc/type.h"
#include"header/stringproc.h"
#include"header/innercommand.h"
#include"header/shc.h"

/*所有的特殊意义字符
    '|', ';', '#', ' ',
    '\t','\'','"', '<',
    '>', 
-----------------------
    '$', '`', '&', '(',
    ')', '{', '}',
*/

static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static PIPE_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static PART_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static int status;/*命令返回状态*/
static char cmd_cache[CMDLINE_MAX];

void processcmd(char *);
static int separatecmd(char *);
static void pipeprocess(PART_ARG);
static void redirect(PIPE_ARG);
static char *preprocess(char *);