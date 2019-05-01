#include"header/_shc.h"

int main(int argc,char *argv[])
{
    char cmd[CMDLINE_MAX];
    int pid,len,mutual,i,status;
    FILE *input[MAX_INPUT_FILE];
    pid=fork();
    if(pid<0)
        exit(-1);
    else if(pid==0)
    {
        // setsid();
        if(argc==1)
        {
            argc=2;/*等于多一个标准输入文件作参数*/
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
        for(i=1;i<argc&&i<MAX_INPUT_FILE;++i)
        {
            while(1)
            {
                if(getppid()==1)
                    exit(1);
                if(mutual)
                    printf("shc:%s$",getcwd(cmd,CMDLINE_MAX));
                if(fgets(cmd,CMDLINE_MAX,input[i-1])==NULL)
                {
                    break;
                }
                len=strlen(cmd);
                if(cmd[len-1]=='\n')
                    cmd[len-1]='\0';/*删除换行符*/
                processcmd(cmd,input[i-1]);
            }
        }
        exit(0);
    }
    wait(&status);
    return WEXITSTATUS(status);
}