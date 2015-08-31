/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : fork_strcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 使用fork()的str_cli函数
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "str_lic.h"

void tu16_10str_cli(FILE *fp, int sockfd)
{
    pid_t   pid;
    char    sendline[MAXLINE] = {0};
    char    recvline[MAXLINE] = {0};

    //child: server->stdout
    if ( (pid = my_fork()) == 0 )
    {
        while (my_readline(sockfd, recvline, MAXLINE) > 0)
        {
             my_fputs(recvline, stdout);
        }

        kill(getppid(), SIGTERM);
        exit(0);
    }

    //parent: stdin->server
    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
        my_writen(sockfd, sendline, strlen(sendline));
    }

    my_shutdown(sockfd, SHUT_WR);
    pause();            //睡眠, 等子进程结束它
    return ;
}
