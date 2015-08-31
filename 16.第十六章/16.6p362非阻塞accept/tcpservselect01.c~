/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpservselect01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/15
* Description  : 在图6-21和6-22基础上加点东西;
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int main(int argc ,char **argv)
{
    int     i = 0;
    int     maxi = 0;
    int     maxfd = 0;
    int     listenfd = 0;
    int     connfd = 0;
    int     sockfd = 0;
    int     nready =0 ;
    int     client[FD_SETSIZE];
    ssize_t n;
    fd_set  rset ;
    fd_set  allset;
    char    buf[MAXLINE];
    socklen_t      clilen;
    struct  sockaddr_in cliaddr;
    struct  sockaddr_in servaddr;

    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    my_bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    my_listen(listenfd, LISTENQ);

    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++)
    {
         client[i] = -1;                //初始化数组;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);          //设置第一个select对象

    while(1)
    {
         rset = allset;
         nready = my_select(maxfd + 1, &rset, NULL, NULL, NULL);

         if (FD_ISSET(listenfd, &rset))
         {
         	 printf("listening socket readable\n"); //+
         	 sleep(5); 								//+
             clilen = sizeof(cliaddr);
             connfd = my_accept(listenfd, (SA *) &cliaddr, &clilen);

             for (i = 0; i < FD_SETSIZE; i++)
             {
                  if (client[i] < 0)
                  {
                      client[i] = connfd;
                      break;
                  }
             }

             if ( FD_SETSIZE == i )
             {
                  err_quit("too many clients");
             }

             FD_SET(connfd, &allset);
             if (connfd > maxfd)
             {
                 maxfd = connfd;
             }
             if (i > maxi)
             {
                  maxi = i;
             }

             if (--nready == 0)
             {
                 continue;
             }
         }

         for (i = 0; i <= maxi; i++)
         {
              if ((sockfd = client[i]) < 0)
              {
                  continue;
              }

              if ( FD_ISSET( sockfd, &rset ) )
              {
                   if ( ( n = my_read(sockfd, buf, MAXLINE) ) == 0 )
                   {
                       my_close(sockfd);
                       FD_CLR( sockfd, &allset );
                       client[i] = -1;
                   }
                   else
                   {
                        my_writen(sockfd, buf, n);
                   }

                   if ( --nready <= 0 )
                   {
                       break;
                   }
              }
         }
    }


}


