#include"processcmd.h"

/*所有的特殊意义字符
    '>', '<', '$', '`',
    '&', '(', ')', ' ',
    '\t','{', '}', '"',
    '\''
*/

char *cmd_arg[MAX_ARG];

void separatecmd(char *,char *[]);

/*
处理命令
*/
int processcmd(char *cmd)
{
    char *p;
    pid_t pid;

    p=strrchr(cmd,'/');
    p=p?p:cmd;/*防止p为空指针*/

    separatecmd(cmd,cmd_arg);
    pid=fork();/*程序在此处分界*/
    if(pid<0)
        printf("error in executing \"%s\"\n",cmd);
    else if(pid==0)/*子程序*/
    {
        if(execvp(*cmd_arg,cmd_arg))
        {
            printf("error in executing \"%s\"\n",cmd);
            exit(-1);
        }
        /*子程序的终止处*/
    }
    /*父程序*/
    printf("waiting\n");
    wait(NULL);
    int i=0;
    while(cmd_arg[i])
    printf("%s\n",cmd_arg[i++]);
    printf("waited\n");
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
            *(cmd_arg+i)=NULL;
            break;
        }
        *(cmd_arg+i)=cmd;
        while(*cmd!=' '&&*cmd!='\t')
            ++cmd;
    }
    return;
}