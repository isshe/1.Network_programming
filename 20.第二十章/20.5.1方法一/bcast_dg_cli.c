/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : bcast_dg_cli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/25
* Description  : 在循环内执行期间阻塞信号
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "bcast_dg_cli.h"

static void recvfrom_alarm(int);

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n = 0;
    const   int     on = 1;
    char    sendline[MAXLINE] = {0};
    char    recvline[MAXLINE + 1] = {0};
    sigset_t   sigset_alrm;
    socklen_t   len ;
    struct  sockaddr    *preply_addr;

    preply_addr = my_malloc(servlen);       //???为什么要这个?
    my_setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    my_signal(SIGALRM, recvfrom_alarm);
    
    my_sigemptyset(&sigset_alrm);
    my_sigaddset(&sigset_alrm, SIGALRM);

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
         my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

         alarm(5);

         while(1)
         {
              len = servlen;
//              DEBUG;
//              printf("recvfrom 前面!\n");
              my_sigprocmask(SIG_UNBLOCK, &sigset_alrm, NULL);
              n = recvfrom(sockfd, recvline, MAXLINE,
                            0, preply_addr, &len);
              my_sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);

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
                   sleep(1);
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
    return ;
}
