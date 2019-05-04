#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>

#include"etc/opt.h"
#include"etc/type.h"
#include"etc/error.h"
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

static char cmd_cache[CMDLINE_MAX];
static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static CMD_ARG cmd_arg_cache[MAX_ARG];
static PIPE_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_pipe_cache[MAX_ARG];
static PART_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_part_cache[MAX_ARG];
static int status;/*命令返回状态*/

void processcmd(char *);
static int separatecmd(char *);
static void pipeprocess(PART_ARG);
static void redirect(PIPE_ARG);
static void preprocess(char *);