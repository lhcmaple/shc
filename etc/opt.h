#ifndef _OPT_H
#define _OPT_H

#define CMDLINE_MAX 65536 /*最多支持长为65535的命令*/
#define MAX_ARG 1025 /*最多支持有1024个分隔的字符组命令,最后一个用来指示结束*/
#define MAX_CMD_LEN 33 /*内置命令的最长长度为32*/
#define MAX_INPUT_FILE 32

#define FUNC_INNERCOMMAND/*内置命令功能,cd与exit为特殊内置命令，不可取消*/
#define FUNC_VARIABLE/*变量解析功能$*/

#endif