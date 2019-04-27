#include<stdio.h>

int main(int argc,char *argv[])
{
    char buf[100];
    for(int i=0;i<argc;++i)
    {
        printf("%s ",argv[i]);
    }
    printf("\n");
    while(fgets(buf,100,stdin))
        printf("%s",buf);
    return -1;
}