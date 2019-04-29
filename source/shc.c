#include"shc.h"

#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

#include"opt.h"
#include"processcmd.h"

int main(int argc,char *argv[])
{
    char cmd[CMDLINE_MAX];
    int pid;

    pid=fork();
    if(pid<0)
        exit(-1);
    else if(pid==0)
    {
        system("clear");
        // setsid();
        // printf("%d--%d",getpid(),getsid(0));
        while(1)
        {
            printf("shc:%s$",getcwd(cmd,CMDLINE_MAX));
            if(fgets(cmd,CMDLINE_MAX,stdin)==NULL)
            {
                exit(-1);
            }
            cmd[strlen(cmd)-1]='\0';/*删除换行符*/
            processcmd(cmd);
        }
    }
    wait(NULL);
    return 0;
}