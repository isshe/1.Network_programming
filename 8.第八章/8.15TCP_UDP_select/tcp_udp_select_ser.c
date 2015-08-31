/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcp_udp_select_ser.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/17
* Description  : 本服务器程序,结合了TCP和UDP.
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

#define max(x, y) ( (x) > (y) ? (x) : (y) )

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

//信号处理函数
void    sig_chld()             //
{
    printf("这里是信号处理函数\n");
}

int main(int argc, char **argv)
{
    int     listenfd = 0;
    int     connfd = 0;
    int     udpfd = 0;
    int     nready = 0;
    int     maxfdp1 = 0;
    char    msg[MAXLINE];
    pid_t   childpid;
    fd_set  rset ;
    ssize_t n;
    socklen_t   len;
    const   int on = 1;
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;
//    void    sig_chld(int);                  //这个函数不懂!!!

    //create listening TCP socket
    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //打开地址复用 on = 1;
    my_setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    my_bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    my_listen(listenfd, LISTENQ);

    //create UDP socket
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    my_bind(udpfd, (SA *)&servaddr, sizeof(servaddr));

    //信号
    my_signal(SIGCHLD, sig_chld);   //tcp客户端子进程退出的时候调用

    //初始化集合
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;

    while (1)
    {
         FD_SET(listenfd, &rset);
         FD_SET(udpfd, &rset);

         if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0 )
         {
             if (errno == EINTR)
             {
                 continue;
             }
             else
             {
                err_sys("select error");
             }
         }//if

         //TCP
         if(FD_ISSET(listenfd, &rset))
         {
              len = sizeof(cliaddr);
              connfd = my_accept(listenfd, (SA *) &cliaddr, &len);

              if ( (childpid = my_fork()) == 0 )
              {
                   my_close(listenfd);
                   str_echo(connfd);
                   exit(0);
              }
              my_close(connfd);
         }

         //UDP
         if (FD_ISSET(udpfd, &rset))
         {
              len = sizeof(cliaddr);
              n = my_recvfrom(udpfd, msg, MAXLINE, 0,
                                (SA *) &cliaddr, &len);

              sendto(udpfd, msg, n, 0, (SA *) &cliaddr, len);
         }
    }

    return 0;
}
