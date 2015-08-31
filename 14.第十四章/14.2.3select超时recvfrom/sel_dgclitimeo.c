/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sel_dgclitimeo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 使用select()为recvfrom()超时
* Function list: 1.readable_timeo();        //select 超时
*                2.dg_cli();                //调用readable_timeo()
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int readable_timeo(int fd, int sec)
{
     fd_set     rset;
     struct     timeval     tv;

     FD_ZERO(&rset);
     FD_SET(fd, &rset);

     tv.tv_sec = sec;
     tv.tv_usec = 0;


     //select()返回准备好的套接字描述符个数
     return (select(fd+1, &rset, NULL, NULL, &tv));
}


void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
     int    n = 0;
     char   sendline[MAXLINE] = {0};
     char   recvline[MAXLINE] = {0};

     while (my_fgets(sendline, MAXLINE, fp) != NULL)
     {
         my_sendto(sockfd, sendline, strlen(sendline),
                   0, pservaddr, servlen);

         if (readable_timeo(sockfd, 5) == 0)
         {
             fprintf(stderr, "socket timeout\n");
         }
         else
         {
              n = my_recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
              recvline[n] = 0;      //null terminate
              my_fputs(recvline, stdout);
         }
     }

}
