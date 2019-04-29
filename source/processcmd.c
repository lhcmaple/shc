#include"processcmd.h"

#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>

#include"opt.h"
#include"type.h"
#include"stringproc.h"
#include"innercommand.h"

/*所有的特殊意义字符
    '>', '<', '$', '`',
    '&', '(', ')', ' ',
    '\t','{', '}', '"',
    '\'','|', ';',
*/

extern INNER_FUNC inner_func[];/*内置命令函数指针*/

static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static PCMD_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static PPCMD_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static int status;/*命令返回状态*/
static int separatecmd(char *);
static void pipeprocess(char ***pipe);

/*
处理命令
*/
void processcmd(char *cmd)
{
    pid_t pid;
    int i;

    separatecmd(cmd);

    if(cmd_arg[0])
    {
        for(i=0;cmd_part[i];++i)/*分号进程*/
        {
            pid=fork();
            if(pid<0)
                fprintf(stderr,"error to fork\n");
            else if(pid==0)/*子进程*/
            {
                pipeprocess(cmd_part[i]);
                /*子进程终止处*/
            }
            if(cmd_part[i][1]==NULL)
            {
                if(strcmp("cd",cmd_part[i][0][0])==0)
                {
                    chdir(cmd_part[0][0][1]);
                }
                else if(strcmp("exit",cmd_part[i][0][0])==0)
                {
                    exit(strtoint(cmd_part[i][0][1]));
                }
            }
            setpgid(pid,pid);
            wait(&status);/*父进程*/
            status=WEXITSTATUS(status);/*取低8位的无符号返回状态值0~255*/
        }
    }
}

/*
分隔命令且检查是否有非法字符
ex:
    echo abc|echo efg|echo f;echo d;echo d|echo f
    ^    ^   ^    ^   ^    ^ ^    ^ ^    ^ ^    ^
    |    |   |    |   |    | |    | |    | |    |
arg |    |  n|    |  n|    |n|    |n|    |n|    |n
    ^        ^        ^      ^      ^      ^
    |        |        |      |      |      |
pipe|        |        |     n|     n|      |     n
    ^                        ^      ^
    |                        |      |
part|                        |      |            n
*/
int separatecmd(char *cmd)
{
    int i_arg=0,i_pipe=0,i_part=0;

    cmd_pipe[i_pipe]=&cmd_arg[i_arg];
    cmd_part[i_part]=&cmd_pipe[i_pipe];
    ++i_pipe;
    ++i_part;

    while(1)
    {
        cmd=delfrontspace(cmd);
        if(*cmd=='\0')
            break;
        if(*cmd=='|')
        {
            *(cmd++)='\0';
            cmd=delfrontspace(cmd);
            cmd_arg[i_arg++]=NULL;
            cmd_pipe[i_pipe]=&cmd_arg[i_arg];
            ++i_pipe;
        }
        if(*cmd==';')
        {
            *(cmd++)='\0';
            cmd=delfrontspace(cmd);
            cmd_arg[i_arg++]=NULL;
            cmd_pipe[i_pipe++]=NULL;
            cmd_pipe[i_pipe]=&cmd_arg[i_arg];
            cmd_part[i_part]=&cmd_pipe[i_pipe];
            ++i_pipe;
            ++i_part;
        }
        cmd_arg[i_arg]=cmd;
        ++i_arg;
        while(*cmd&&*cmd!=' '&&*cmd!='\t'&&*cmd!='|'&&*cmd!=';')
            ++cmd;
    }
    cmd_arg[i_arg]=NULL;
    cmd_pipe[i_pipe]=NULL;
    cmd_part[i_part]=NULL;
    return 0;
}

void pipeprocess(PPCMD_ARG pprocess)
{
    pid_t pid;
    int filedes[2];

    setpgid(0,0);
    while(*(pprocess+1))
    {
        if(pipe(filedes))
            printf("error to create a pipe\n");
        pid=fork();
        if(pid<0)
            fprintf(stderr,"error to fork\n");
        else if(pid==0)/*子子进程*/
        {
            close(filedes[0]);
            if(dup2(filedes[1],STDOUT_FILENO)!=STDOUT_FILENO)
                fprintf(stderr,"error to dup2\n");
            close(filedes[1]);
            inner_func[innercommand(**pprocess)](*pprocess);
            /*子子进程终止处*/
        }
        close(filedes[1]);
        if(dup2(filedes[0],STDIN_FILENO)!=STDIN_FILENO)
            fprintf(stderr,"error to dup2\n");
        close(filedes[0]);
        ++pprocess;
    }
    inner_func[innercommand(**pprocess)](*pprocess);
    /*子子进程终止处*/
}