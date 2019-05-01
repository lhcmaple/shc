#include<stdio.h>
#include<unistd.h>
#include<termios.h>

int main(int argc,char *argv[])
{
    char *passwd=getpass("Input your password:");
    printf("Your password is:%s\n",passwd);
    return 0;
}