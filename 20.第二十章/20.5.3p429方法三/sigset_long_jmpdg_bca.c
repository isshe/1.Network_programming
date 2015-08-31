/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sigset_long_jmpdg_bca.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/26
* Description  : 从信号处理函数中使用sigsetjmp()和siglongjmp(),
*                   为了保证健壮性, 应避免使用siglongjmp(),
*                   而改用pselect()或IPC方法;
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

#include <setjmp.h>

static void recvfrom_alarm(int);

static sigjmp_buf jmpbuf;       //分配一个跳转缓冲区

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
     int    n = 0;
     const  int     on = 1;
     char   sendline[MAXLINE] = {0};
     char   recvline[MAXLINE] = {0};
     socklen_t  len;
     struct sockaddr    *preply_addr;        //用于保存接收到的服务信息?!

     preply_addr = my_malloc(servlen);

     my_setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

     my_signal(SIGALRM, recvfrom_alarm);

     while (my_fgets(sendline, MAXLINE, fp) != NULL)
     {
          my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

          alarm(2);

          while (1)
          {
               if (sigsetjmp(jmpbuf, 1) != 0)
               {
                   printf("sigsetjmp 里面!\n");
                   break;
               }

               len = servlen;
               printf("recvfrom 前面!\n");
               n = my_recvfrom(sockfd, recvline, MAXLINE,
                                0, preply_addr, &len);
               recvline[n] = 0;

               printf("from %s: %s",
                       my_sock_ntop(preply_addr, len), recvline);
          }
     }

     free(preply_addr);


}

static void recvfrom_alarm(int signo)
{
    printf("时间到!\n");
     siglongjmp(jmpbuf, 1);
}
