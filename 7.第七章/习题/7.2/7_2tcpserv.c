/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpserv01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/12
* Description  : TCP回射服务器程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

//#define SERV_PORT 8888
//#define LISTENQ 1024

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



int main(int argc, char **argv)
{
    int     listenfd = 0;
    int     connfd = 0;
    pid_t   childpid ;
    socklen_t   clilen;
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;

    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    my_bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    my_listen(listenfd, LISTENQ);

    while(1)
    {
        clilen = sizeof(cliaddr);
        connfd = my_accept(listenfd, (SA*)&cliaddr, &clilen);

        if ((childpid = my_fork()) == 0)
        {
            my_close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        my_close(connfd);
    }

    return 0;
}

