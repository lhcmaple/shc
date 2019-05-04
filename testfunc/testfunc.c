#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<termios.h>
#include<string.h>
#include"etc/type.h"
#include"etc/error.h"
#include"etc/opt.h"

#define in stdin

static char cmd_cache[CMDLINE_MAX];
static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static CMD_ARG cmd_arg_cache[MAX_ARG];
static PIPE_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_pipe_cache[MAX_ARG];
static PART_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static CMD_ARG cmd_part_cache[MAX_ARG];
static int status;/*命令返回状态*/

static char *preprocess(char *cmd)
{
    int isspace=1,len;
    char *p_cache=cmd_cache;
    CMD_ARG *p_arg_cache=cmd_arg_cache;
    CMD_ARG *p_pipe_cache=cmd_pipe_cache;
    CMD_ARG *p_part_cache=cmd_part_cache;
    while(*cmd)
    {
        switch(*cmd)
        {
            case ' ':
            case '\t':
                isspace=1;
                *cmd='\0';
                *(p_cache++)=*cmd;
                break;
            case '|':
                isspace=1;
                *cmd='\0';
                *(p_pipe_cache++)=p_cache;
                *(p_cache++)=*cmd;
                break;
            case ';':
                isspace=1;
                *cmd='\0';
                *(p_pipe_cache++)=p_cache;
                *(p_part_cache++)=p_cache;
                *(p_cache++)=*cmd;
                break;
            case '"':
                if(isspace)
                {
                    isspace=0;
                    *(p_arg_cache++)=p_cache;
                }
                while(*(++cmd)!='"')
                {
                    if(*cmd=='\0')
                    {
                        *cmd='\n';
                        printf(">");
                        if(fgets(cmd+1,CMDLINE_MAX,in)==NULL)
                        {
                            exit(NORMAL_EXIT);
                        }
                        len=strlen(cmd);
                        if(cmd[len-1]=='\n')
                            cmd[len-1]='\0';/*删除换行符*/
                    }
                    *(p_cache++)=*cmd;
                }
                break;
            case '\'':
                if(isspace)
                {
                    isspace=0;
                    *(p_arg_cache++)=p_cache;
                }
                while(*(++cmd)!='\'')
                {
                    if(*cmd=='\0')
                    {
                        *cmd='\n';
                        printf(">");
                        if(fgets(cmd+1,CMDLINE_MAX,in)==NULL)
                        {
                            exit(NORMAL_EXIT);
                        }
                        len=strlen(cmd);
                        if(cmd[len-1]=='\n')
                            cmd[len-1]='\0';/*删除换行符*/
                    }
                    *(p_cache++)=*cmd;
                }
                break;
            default:
                if(isspace)
                {
                    isspace=0;
                    *(p_arg_cache++)=p_cache;
                }
                *(p_cache++)=*cmd;
        }
        ++cmd;
    }
    *p_cache='\0';
    *p_arg_cache=NULL;
    *p_pipe_cache=NULL;
    *p_part_cache=NULL;
    return cmd_cache;
}

int main(int argc,char *argv[])
{
    int i=0;
    char cmd[100]="echo abc||adke\"dk\" dkei |\"abc";
    preprocess(cmd);
    while(cmd_arg_cache[i])
    {
        printf("%s\n",cmd_arg_cache[i]);
        i++;
    }
    return 0;
}