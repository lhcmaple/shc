#include<stdio.h>
#include<unistd.h>
#include<termios.h>

int main(int argc,char *argv[])
{
    char *p;
    p=ttyname(STDIN_FILENO);
    if(p)
        printf("%s\n",p);
    return -1;
}