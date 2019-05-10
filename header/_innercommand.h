#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include"etc/opt.h"
#include"etc/type.h"
#include"header/stringproc.h"

int innercommand(char *);
static void exec(void *);
static void icd(void *);
static void iexit(void *);
#ifdef FUNC_INNERCOMMAND
static void ijobs(void *);
#endif

/*内置命令
    "cd",
    "exit",
    "jobs"
*/
static char inner_cmd[][MAX_CMD_LEN]={
    "cd"
    ,"exit"
#ifdef FUNC_INNERCOMMAND
    ,"jobs"
#endif
    ,""};/*空字符串表明结束*/

INNER_FUNC inner_func[]={
    exec
    ,icd
    ,iexit
#ifdef FUNC_INNERCOMMAND
    ,ijobs
#endif
};/*对齐isinnnercommand的返回参数*/