#include"stringproc.h"

char *delfrontspace(char *cmd)
{
    while(cmd==' '||cmd=='\t')
        ++cmd;
    return cmd;/*此处cmd可能指向'\0'*/
}