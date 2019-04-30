#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>

#include"opt.h"
#include"processcmd.h"

int main(int argc,char *argv[])
{
    char cmd[CMDLINE_MAX];
    int len;
    while(1)
    {
        if(getppid()==1)
            exit(-1);
        printf("shc:%s$",getcwd(cmd,CMDLINE_MAX));
        if(fgets(cmd,CMDLINE_MAX,stdin)==NULL)
        {
            exit(-1);
        }
        len=strlen(cmd);
        if(cmd[len-1]=='\n')
            cmd[len-1]='\0';/*删除换行符*/
        processcmd(cmd);
    }
}