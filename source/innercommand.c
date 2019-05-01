#include"header/_innercommand.h"

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

static void exec(void *arg)
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

static void icd(void *arg)
{
    exit(0);
}

static void iexit(void *arg)
{
    exit(0);
}

#if FUNC_INNERCOMMAND
static void ijobs(void *arg)
{
    char** pprocess=(char **)arg;
    
    exit(0);
}
#endif