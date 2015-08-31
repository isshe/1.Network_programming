/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : SO_dgclitimeo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 使用SO_RCVTIMEO套接字选项设置超时的dgcli函数
* Function list: 1. dg_cli()
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t serlen)
{
     int    n = 0;
     char   sendline[MAXLINE] = {0};
     char   recvline[MAXLINE] = {0};
     struct timeval     tv;

     tv.tv_sec  = 5;
     tv.tv_usec = 0;

     my_setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

     while (my_fgets(sendline, MAXLINE, fp) != NULL)
     {
          my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, serlen);

          if ( (n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0 )
          {
               if (EWOULDBLOCK == errno)    //E= error; 可能被阻塞
               {
                   fprintf(stderr, "socket timeout");
                   continue;
               }
               else
               {
                    err_sys("recvfrom error");
               }
          }

          recvline[n] = 0;
          my_fputs(recvline, stdout);
     }
}
