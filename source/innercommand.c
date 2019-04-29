#include"innercommand.h"

#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include"opt.h"
#include"type.h"
#include"stringproc.h"

static void exec(void *);
static void icd(void *);
static void iexit(void *);
#if FUNC_INNERCOMMAND
static void ijobs(void *);
#endif

/*内置命令
    "cd",
    "exit",
    "jobs"
*/

static char inner_cmd[][MAX_CMD_LEN]={
    "cd"
    ,"exit"
#if FUNC_INNERCOMMAND
    ,"jobs"
#endif
    ,""};/*空字符串表明结束*/

INNER_FUNC inner_func[]={
    exec
    ,icd
    ,iexit
#if FUNC_INNERCOMMAND
    ,ijobs
#endif
};/*第一个NULL只是为了对齐isinnnercommand的返回参数*/

int innercommand(char *icmd)
{
    int i;
    for(i=0;inner_cmd[i][0]!='\0';++i)
    {
        if(strcmp(icmd,inner_cmd[i])==0)
        {
            return i+1;
        }
    }
    return 0;
}

void exec(void *arg)
{
    char **pprocess;
    char *p;
    pprocess=(char **)arg;
    p=pprocess[0];
    pprocess[0]=strrchr(p,'/');
    pprocess[0]=pprocess[0]?pprocess[0]:p;/*防止为空指针*/
    if(execvp(p,pprocess))
    {
        fprintf(stderr,"error in executing \"%s\"\n",p);
        exit(-1);
    }
}

void icd(void *arg)
{
    exit(0);
}

void iexit(void *arg)
{
    char **pprocess=(char **)arg;
    exit(strtoint(pprocess[1]));
}

#if FUNC_INNERCOMMAND
void ijobs(void *arg)
{
    char** pprocess=(char **)arg;
    
    exit(0);
}
#endif