#include"processcmd.h"

/*所有的特殊意义字符
    '>', '<', '$', '`',
    '&', '(', ')', ' ',
    '\t','{', '}', '"',
    '\'','|', ';',
*/

char *cmd_arg[MAX_ARG];/*字符指针数组*/
char **cmd_pipe[MAX_ARG];/*管道分隔的命令指针*/
char ***cmd_part[MAX_ARG];/*分号分隔的命令指针*/

int separatecmd(char *);

/*
处理命令
*/
int processcmd(char *cmd)
{
    char *p;
    pid_t pid;

    separatecmd(cmd);
    if(cmd_arg[0])
    {
        int i=0;
        while(cmd_part[i])
        {
            printf("%s\n",**cmd_part[i]);
            sleep(1);
            ++i;
        }
        printf("\n");
        i=0;
        while(cmd_pipe[i])
        {
            printf("%s\n",*cmd_pipe[i]);
            sleep(1);
            ++i;
        }
        printf("\n");
        i=0;
        while(cmd_arg[i])
        {
            printf("%s\n",cmd_arg[i]);
            sleep(1);
            ++i;
        }
    }
    // pid=fork();/*程序在此处分界*/
    // if(pid<0)
    //     printf("error in executing \"%s\"\n",cmd);
    // else if(pid==0)/*子程序*/
    // {
    //     p=cmd_arg[0];
    //     cmd_arg[0]=strrchr(p,'/');
    //     cmd_arg[0]=cmd_arg[0]?cmd_arg[0]:p;/*防止为空指针*/
    //     if(execvp(cmd_arg[0],cmd_arg))
    //     {
    //         printf("error in executing \"%s\"\n",cmd);
    //         exit(-1);
    //     }
    //     /*子程序的终止处*/
    // }
    // /*父程序*/
    // wait(NULL);
    return 0;/*默认返回状态为0*/
}

/*
分隔命令且检查是否有非法字符
*/
int separatecmd(char *cmd)
{
    int i_arg=0,i_pipe=0,i_part=0;

    // cmd=delfrontspace(cmd);
    // if(*cmd=='\0')/*命令全为空格和Tab*/
    // {
    //     cmd_arg[0]=NULL;
    //     cmd_pipe[0]=NULL;
    //     cmd_part[0]=NULL;
    //     return 0;
    // }

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
            cmd_pipe[i_pipe]=&cmd_arg[i_arg];
            ++i_pipe;
        }
        if(*cmd==';')
        {
            *(cmd++)='\0';
            cmd=delfrontspace(cmd);
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