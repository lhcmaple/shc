#include"processcmd.h"

/*所有的特殊意义字符
    '>', '<', '$', '`',
    '&', '(', ')', ' ',
    '\t','{', '}', '"',
    '\'','|', ';',
*/

CMD_ARG cmd_arg[MAX_ARG];/*字符指针数组,不设越界控制*/
PCMD_ARG cmd_pipe[MAX_ARG];/*管道分隔的命令指针,不设越界控制*/
PPCMD_ARG cmd_part[MAX_ARG];/*分号分隔的命令指针,不设越界控制*/

int separatecmd(char *);
int pipeprocess(char ***pipe);

/*
处理命令
*/
int processcmd(char *cmd)
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
            wait(NULL);/*父进程*/
        }
    }
    return 0;/*默认返回状态为0*/
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

int pipeprocess(PPCMD_ARG pprocess)/*可重入*/
{
    pid_t pid;
    int filedes[2];
    char *p;

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
            p=**pprocess;
            **pprocess=strrchr(p,'/');
            **pprocess=**pprocess?**pprocess+1:p;/*防止为空指针*/
            if(execvp(p,*pprocess))
            {
                fprintf(stderr,"error in executing \"%s\"\n",p);
                exit(-1);
            }
            /*子子进程终止处*/
        }
        close(filedes[1]);
        if(dup2(filedes[0],STDIN_FILENO)!=STDIN_FILENO)
            fprintf(stderr,"error to dup2\n");
        close(filedes[0]);
        ++pprocess;
    }
    p=**pprocess;
    **pprocess=strrchr(p,'/');
    **pprocess=**pprocess?**pprocess+1:p;/*防止为空指针*/
    if(execvp(p,*pprocess))
    {
        fprintf(stderr,"error in executing \"%s\"\n",p);
        exit(-1);
    }
    return 0;
    /*子进程终止处*/
}