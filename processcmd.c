#include"processcmd.h"

/*所有的特殊意义字符
    '>', '<', '$', '`',
    '&', '(', ')', ' ',
    '\t','{', '}', '"',
    '\''
*/

char *cmd_arg[MAX_ARG];/*字符指针数组*/

void separatecmd(char *,char *[]);

/*
处理命令
*/
int processcmd(char *cmd)
{
    char *p;
    pid_t pid;

    separatecmd(cmd,cmd_arg);
    pid=fork();/*程序在此处分界*/
    if(pid<0)
        printf("error in executing \"%s\"\n",cmd);
    else if(pid==0)/*子程序*/
    {
        p=cmd_arg[0];
        cmd_arg[0]=strrchr(p,'/');
        cmd_arg[0]=cmd_arg[0]?cmd_arg[0]:p;/*防止为空指针*/
        if(execvp(cmd_arg[0],cmd_arg))
        {
            printf("error in executing \"%s\"\n",cmd);
            exit(-1);
        }
        /*子程序的终止处*/
    }
    /*父程序*/
    wait(NULL);
    return 0;/*默认返回状态为0*/
}

/*
分隔命令且检查是否有非法字符
*/
void separatecmd(char *cmd,char *cmd_arg[])
{
    for(int i=0;i<MAX_ARG-1;++i)
    {
        while(*cmd==' '||*cmd=='\t')
            *(cmd++)='\0';
        if(*cmd=='\0')
        {
            cmd_arg[i]=NULL;
            break;
        }
        cmd_arg[i]=cmd;
        while(*cmd!=' '&&*cmd!='\t'&&*cmd)/*跳到下一参数*/
            ++cmd;
    }
    return;
}