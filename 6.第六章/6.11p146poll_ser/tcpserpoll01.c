/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpserpoll01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/16
* Description  : 使用poll()的服务器程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"
//#include <linux/limits.h>         //for OPEN_MAX, linux已经把这个取消了
#define OPEN_MAX 1024
#define INFTIM   -1

int main(int argc, char **argv)
{
    int     i =0 ;
    int     maxi = 0;
    int     listenfd = 0;
    int     connfd = 0;
    int     sockfd = 0;
    int     nready = 0;
    ssize_t n ;
    char    buf[MAXLINE] = {0};
    socklen_t       clilen;
    struct  pollfd  client[OPEN_MAX];
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;

    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    my_bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    my_listen(listenfd, LISTENQ);

    //把listenfd放第一个, 其他的全部初始化为 -1
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;          //普通读

    for (i = 1; i < OPEN_MAX; i++)
    {
        client[i].fd = -1;
    }
    maxi = 0;

    while (1)
    {
        nready = my_poll(client, maxi + 1, INFTIM);          //INFTIM 永久等待

        if (client[0].revents & POLLRDNORM)
        {
             clilen = sizeof(cliaddr);
             connfd = my_accept(listenfd, (SA *)&cliaddr, &clilen);

             for (i = 1; i < OPEN_MAX; i++)
             {
                 if (client[i].fd < 0)
                 {
                      client[i].fd = connfd;
                      break;
                 }
             }

             if (OPEN_MAX == i)
             {
                  err_quit("too many clients");
             }

             client[i].events = POLLRDNORM;
            if (i > maxi)
            {
                maxi = i;
            }
            if ( --nready <= 0 )
            {
                 continue;
            }
        } //if

        for (i = 1; i <= maxi; i++)
        {
            if ( (sockfd = client[i].fd) < 0 )
            {
                 continue;
            }
            //相当于else
            if( client[i].revents & (POLLRDNORM | POLLERR) )
            {
                if ( (n = read(sockfd, buf, MAXLINE)) < 0 )
                {
                     if ( ECONNRESET == errno )
                     {
                         //connection reset by client
                         my_close(sockfd);
                         client[i].fd = -1;
                     }
                     else
                     {
                         err_sys("read error");
                     }
                }
                else if ( 0 == n )
                {
                    //connection closed by client
                    my_close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                     my_writen(sockfd, buf, n);
                }

                if ( --nready <= 0 )
                {
                    break;              //no more readable descriptors
                }
            }
        }
    }

    return 0;
}
