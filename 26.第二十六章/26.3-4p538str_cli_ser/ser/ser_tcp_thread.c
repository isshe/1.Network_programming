/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : ser_tcp_thread.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/28
* Description  : 使用线程的TCP回射服务器函数
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"


static void *doit(void*);

void str_echo(int sockfd);

int main(int argc, char **argv)
{
    int     listenfd = 0;
    int     connfd = 0;
    pthread_t   tid;
    socklen_t   addrlen;
    socklen_t   len ;
    struct  sockaddr *cliaddr = NULL;

    if (argc == 2)
    {
         listenfd = tcp_listen(NULL, argv[1], &addrlen);
    }
    else if (argc == 3)
    {
        listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    }
    else
    {
         err_quit("usage: ./a.out [ <host> ] <service or port>");
    }

    cliaddr = my_malloc(addrlen);

    while (1)
    {
         len = addrlen;
         connfd = my_accept(listenfd, cliaddr, &len);
         my_pthread_create(&tid, NULL, &doit, (void*)connfd);
    }
}


static void *doit(void *arg)
{
     my_pthread_detach(pthread_self());

     str_echo((int) arg);

     my_close((int) arg);

     return(NULL);
}

void str_echo(int sockfd)
{
    ssize_t     n ;
    char        buf[MAXLINE];


again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
    {
        my_write(sockfd, buf, n);
    }

    if (n < 0 && errno == EINTR)
    {
        goto again;
    }
    else if (n < 0)
    {
        err_sys("str_echo: read error");
    }
}



