/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : thread_str_cli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/28
* Description  : 使用线程的str_cli函数
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

void *copyto(void *);

static  int     sockfd;     //global for both threads to access
static  FILE    *fp;

void str_cli(FILE *fp_arg, int sockfd_arg)
{
    char    recvline[MAXLINE];
    pthread_t   tid;

    sockfd = sockfd_arg;        //copy arguments to externals
    fp = fp_arg;

    my_pthread_create(&tid, NULL, copyto, NULL);

    while (my_readline(sockfd, recvline, MAXLINE) > 0)
    {
         my_fputs(recvline, stdout);
    }
}


void *copyto(void *arg)
{
     char   sendline[MAXLINE] = {0};

     while (my_fgets(sendline, MAXLINE, fp) != NULL)
     {
         my_write(sockfd, sendline, strlen(sendline));
     }

     my_shutdown(sockfd, SHUT_WR);  //关掉写半部???不对, WR应该是读和写了

     return (NULL);     //返回后线程也终止
}
