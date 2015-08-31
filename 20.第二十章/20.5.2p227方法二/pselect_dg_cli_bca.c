/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : pselect_dg_cli_bca.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/25
* Description  : 使用pselect()阻塞和解阻塞信号
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

static void recvfrom_alarm(int);

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n = 0;
    const   int     on = 1;
    char    sendline[MAXLINE] = {0};
    char    recvline[MAXLINE] = {0};
    fd_set  rset;
    sigset_t    sigset_alrm;
    sigset_t    sigset_empty;
    socklen_t   len;
    struct  sockaddr    *preply_addr;

    preply_addr = my_malloc(servlen);
    my_setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    FD_ZERO(&rset);

    my_sigemptyset(&sigset_empty);
    my_sigemptyset(&sigset_alrm);
    my_sigaddset(&sigset_alrm, SIGALRM);

    my_signal(SIGALRM, recvfrom_alarm);

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
         my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

         my_sigprocmask(SIG_BLOCK, &sigset_alrm, NULL);     //
         alarm(2);

         while (1)
         {
              FD_SET(sockfd, &rset);
              printf("pselect 前面!\n");
              n = pselect(sockfd+1, &rset, NULL, NULL,
                            NULL, &sigset_empty);

              if (n < 0)
              {
                  if (errno == EINTR)
                  {
                      printf("pselect EINTR!\n");
                       break;
                  }
                  else
                  {
                      err_sys("pselect error");
                  }
              }
              else if ( n != 1 )
              {
                   err_sys("pselect error: returned %d", n);
              }

              len = servlen;
              n = my_recvfrom(sockfd, recvline, MAXLINE,
                                0, preply_addr, &len);
              recvline[n] = 0;
              printf("from %s: %s",
                      my_sock_ntop_host(preply_addr, len),
                      recvline);
         }
    }

    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    printf("时间到, 可继续输入:\n");
    return ;
}
