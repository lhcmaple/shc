#include"header/_shc.h"

int main(int argc,char *argv[])
{
    int pid,len,mutual,i,status;

    pid=fork();
    if(pid<0)
        exit(ERROR_FAILTOSTART);/*程序直接结束*/
    else if(pid==0)
    {
        // setsid();
        if(argc==1)
        {
            argc=2;/*等于多一个标准输入作参数*/
            input[0]=stdin;
            mutual=isatty(STDIN_FILENO);
        }
        else
        {
            for(i=1;i<argc&&i<=MAX_INPUT_FILE;++i)
            {
                if((input[i-1]=fopen(argv[i],"r"))==NULL)
                    fprintf(stderr,"error in open \"%s\"\n",argv[i]);
            }
            mutual=0;
        }
        for(i=1;i<argc&&i<=MAX_INPUT_FILE;++i)
        {
            in=input[i-1];
            while(1)
            {
                if(getppid()==1)
                    exit(ERROR_PARENTEXIT);
                if(mutual)
                    printf("shc:%s$",getcwd(cmd,CMDLINE_MAX));
                if(fgets(cmd,CMDLINE_MAX,in)==NULL)
                {
                    break;
                }
                len=strlen(cmd);
                if(cmd[len-1]=='\n')
                    cmd[len-1]='\0';/*删除换行符*/
                processcmd(cmd,in);
            }
        }
        exit(NORMAL_EXIT);
    }
    wait(&status);/*等待子进程结束，并获取状态*/
    return WEXITSTATUS(status);
}