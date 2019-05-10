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
    ')', '{', '}', '=',
*/

#ifdef FUNC_VARIABLE
typedef struct not_variable{
    char *start;
    char *end;
    struct not_variable *prev;
    struct not_variable *next;
}NOT_VARIABLE;
#endif

#ifdef FUNC_VARIABLE
static NOT_VARIABLE *not_var[MAX_ARG];

static NOT_VARIABLE var_cache[MAX_ARG];/*内存分配器的内存池*/
static NOT_VARIABLE *alloc_cache;/*内存分配器的指针*/
#endif

static char cmd_cache[CMDLINE_MAX];
static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static CMD_ARG cmd_arg_cache[MAX_ARG];
static PIPE_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_pipe_cache[MAX_ARG];
static PART_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_part_cache[MAX_ARG];
static int status;/*命令返回状态*/

void processcmd(char *);
static int interpretcmd(char *);
static void pipeprocess(PART_ARG);
static void redirect(PIPE_ARG);

#ifdef FUNC_VARIABLE
static void init_alloc_NOT_VARIABLE();
static NOT_VARIABLE *alloc_NOT_VARIABLE();
#endif