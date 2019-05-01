#include"processcmd.h"

#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>

#include"opt.h"
#include"type.h"
#include"stringproc.h"
#include"innercommand.h"

/*所有的特殊意义字符
    '|', ';', '#', ' ',
    '\t','\'','"',
-----------------------
    '>', '<', '$', '`',
    '&', '(', ')', '{',
    '}',
*/

extern INNER_FUNC inner_func[];/*内置命令函数指针*/

static CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
static PIPE_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
static PPIPE_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/
static int status;/*命令返回状态*/
static int separatecmd(char *,FILE *input);
static void pipeprocess(char ***pipe);
static void redirect(PIPE_ARG arg);

/*
处理命令
*/
void processcmd(char *cmd,FILE *input)
{
    pid_t pid;
    int i;

    separatecmd(cmd,input);

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
int separatecmd(char *cmd,FILE *input)
{
    int i_arg=0,i_pipe=0,i_part=0;
    int flag1=0,flag2=0,len;
    char c;

    cmd_pipe[i_pipe]=&cmd_arg[i_arg];
    cmd_part[i_part]=&cmd_pipe[i_pipe];
    ++i_pipe;
    ++i_part;

    while(1)
    {
        cmd=delfrontspace(cmd);
        if(*cmd=='\0'||*cmd=='#')
        {
            *cmd='\0';
            break;
        }
        if(*cmd=='|')/*管道之间不可为空*/
        {
            *(cmd++)='\0';
            cmd=delfrontspace(cmd);
            cmd_arg[i_arg++]=NULL;
            cmd_pipe[i_pipe]=&cmd_arg[i_arg];
            ++i_pipe;
        }
        if(*cmd==';')/*分号之间不可为空*/
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
        if(*cmd=='\'')
        {
            *(cmd++)='\0';
            flag1=1;
        }
        else if(*cmd=='"')
        {
            *(cmd++)='\0';
            flag2=1;
        }
        cmd_arg[i_arg]=cmd;
        ++i_arg;
        if(flag1||flag2)
        {
            c=flag1?'\'':'"';
            while(*cmd&&*cmd!=c)
                ++cmd;
            if(*cmd==c)
                *(cmd++)='\0';
            else
            {
                *(cmd++)='\n';
                while(1)
                {
                    printf(">");
                    if(fgets(cmd,CMDLINE_MAX,input)==NULL)/*可能越界*/
                        exit(-1);
                    while(*cmd&&*cmd!=c)
                        ++cmd;
                    if(*cmd==c)
                    {
                        *(cmd++)='\0';
                        break;
                    }
                }
                len=strlen(cmd);
                if(cmd[len-1]=='\n')
                    cmd[len-1]='\0';
            }
            flag1=0;
            flag2=0;
        }
        else
        {
            while(
            *cmd&&*cmd!='#'       &&
            *cmd!=' '&&*cmd!='\t' &&
            *cmd!='|'&&*cmd!=';'
            )
            ++cmd;
        }
    }
    cmd_arg[i_arg]=NULL;
    cmd_pipe[i_pipe]=NULL;
    cmd_part[i_part]=NULL;
    return 0;
}

void pipeprocess(PPIPE_ARG pprocess)
{
    pid_t pid;
    int filedes[2];
    char *param;

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
            redirect(*pprocess);
            if(**pprocess)
                inner_func[innercommand(**pprocess)](*pprocess);
                /*子子进程终止处*/
        }
        close(filedes[1]);
        if(dup2(filedes[0],STDIN_FILENO)!=STDIN_FILENO)
            fprintf(stderr,"error to dup2\n");
        close(filedes[0]);
        ++pprocess;
    }
    redirect(*pprocess);
    if(**pprocess)
        inner_func[innercommand(**pprocess)](*pprocess);
        /*子子进程终止处*/
}

void redirect(PIPE_ARG arg)
{
    CMD_ARG param;
    int fd0,fd1;
    while(*arg)
    {
        if(param=strrchr(*arg,'<'))
        {
            *(param++)='\0';
            if(**arg=='\0')
            {
                fd0=0;
            }
            else
            {
                fd0=atoi(*arg);
            }
            if(*param=='&')
            {
                ++param;
                fd1=atoi(param);
            }
            else
            {
                fd1=open(param,O_RDONLY);
            }
            if(dup2(fd1,fd0)!=fd0)
            {
                fprintf(stderr,"failed to redirect %s to %s",*arg,param);
                exit(-1);
            }
            close(fd1);
            *arg=NULL;
        }
        else if(param=strrchr(*arg,'>'))
        {
            *(param++)='\0';
            if(**arg=='\0')
            {
                fd0=1;
            }
            else
            {
                fd0=atoi(*arg);
            }
            if(*param=='&')
            {
                ++param;
                fd1=atoi(param);
            }
            else
            {
                fd1=open(param,O_WRONLY|O_CREAT);
            }
            if(dup2(fd1,fd0)!=fd0)
            {
                fprintf(stderr,"failed to redirect %s to %s",*arg,param);
                exit(-1);
            }
            close(fd1);
            *arg=NULL;
        }
        ++arg;
    }
}