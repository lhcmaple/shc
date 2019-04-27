#include"stringproc.h"

/*
删除前导空格或Tab,并置为'\0'
*/
char *delfrontspace(char *cmd)
{
    while(*cmd==' '||*cmd=='\t')
        *(cmd++)='\0';
    return cmd;/*此处cmd可能指向'\0'*/
}