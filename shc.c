#include"shc.h"

int main(int argc,char *argv[])
{
    char cmd[CMDLINE_MAX];
    while(1)
    {
        printf("shc:%s$",getcwd(cmd,CMDLINE_MAX));
        if(fgets(cmd,CMDLINE_MAX,stdin)==NULL)
        {
            printf("\n");
            continue;
        }
        cmd[strlen(cmd)-1]='\0';/*删除换行符*/
        cstatus=processcmd(cmd);/*命令执行的返回状态*/
    }
    return 0;
}