/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : dgclitimeo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 使用alarm()超时recvfrom()的dg_cli函数
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

static void sig_alrm(int);

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
     int    n = 0;
     char   sendline[MAXLINE] = {0};
     char   recvline[MAXLINE] = {0};

     my_signal(SIGALRM, sig_alrm);

     while (my_fgets(sendline, MAXLINE, fp) != NULL)
     {
          my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

          alarm(5);         //只超时五秒啊?

          if( (n = recvfrom(sockfd, recvline, MAXLINE, 0,
                            0, NULL, NULL)) < 0 )
          {
               if (EINTR == errno)
               {
                   fprintf(stderr, "socket timeout\n");
               }
               else
               {
                    err_sys("recvfrom error");
               }
          }
          else
          {
               alarm(0);            //turn off the alarm
               recvline[n] = 0;
               my_fputs(recvline, stdout);
          }
     }
}

static void
sig_alrm(int signo)
{
    return ;                //just interrupt the recvfrom()
}
