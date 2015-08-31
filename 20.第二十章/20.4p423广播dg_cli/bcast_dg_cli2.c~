/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : bcast_dg_cli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/25
* Description  : 广播请求的dg_cli()函数, 加了点东西, 使它可以连续输入
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "bcast_dg_cli.h"

static void recvfrom_alarm(int);

static int flag;

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n = 0;
    const   int     on = 1;
    char    sendline[MAXLINE] = {0};
    char    recvline[MAXLINE + 1] = {0};
    socklen_t   len ;
    struct  sockaddr    *preply_addr;

    preply_addr = my_malloc(servlen);       //???为什么要这个?

    my_setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    my_signal(SIGALRM, recvfrom_alarm);

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
         my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

         alarm(3);
         
         flag = 0;
         while(1)
         {
              len = servlen;
//              DEBUG;
//              printf("recvfrom 前面!\n");
              
              if (flag == 1)
              {
                   break;
              }
              
              n = recvfrom(sockfd, recvline, MAXLINE,
                            0, preply_addr, &len);

              if (n < 0)
              {
                   if (errno == EINTR)
                   {
                       break;
                   }
                   else
                   {
                        err_sys("recvfrom error");
                   }
              }
              else
              {
                   recvline[n] = 0;
                   printf("from %s: %s",
                           my_sock_ntop_host(preply_addr, len),
                           recvline);
              }
         }//while
    }//while

    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    printf("时间到!\n");
    flag = 1;
    return ;
}
