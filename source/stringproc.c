#include"header/_stringproc.h"

/*
删除前导空格或Tab,并置为'\0'
*/
char *delfrontspace(char *cmd)
{
    while(*cmd==' '||*cmd=='\t')
        *(cmd++)='\0';
    return cmd;/*此处cmd可能指向'\0'*/
}

/*
字符串str转化为0~255的整数值
*/
int strtoint(char *str)
{
    int flag,i=0;
    if(str==NULL)
        return 0;
    if(*str=='-')
    {
        flag=-1;
        ++str;
    }
    while(*str)
    {
        i=i*10+*str-'0';
        ++str;
    }
    return (flag*i)&0xff;
}