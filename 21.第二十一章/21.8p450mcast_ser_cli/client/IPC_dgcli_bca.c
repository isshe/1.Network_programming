/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : IPC_dgcli_bca.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/26
* Description  : 使用从信号处理函数到主控函数的管道作为IPC;
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"


static void recvfrom_alarm(int);
static int  pipefd[2];

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n = 0;
    int     maxfdp1 = 0;
    const   int     on = 1;
    char    sendline[MAXLINE] = {0};
    //+64是因为对端还发送mac地址,和时间过来
    char    recvline[MAXLINE + 64] = {0};       
    fd_set  rset;
    socklen_t   len;
    struct  sockaddr    *preply_addr = NULL;

    preply_addr = my_malloc(servlen);

    my_setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    my_pipe(pipefd);
//    printf("pipefd[0] = %d\n", pipefd[0]); //+

    maxfdp1 = max(sockfd, pipefd[0]) + 1;

    FD_ZERO(&rset);

    my_signal(SIGALRM, recvfrom_alarm);

    while (my_fgets(sendline, MAXLINE, fp) != NULL )
    {
         my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);

         alarm(3);

         while (1)
         {
              FD_SET(sockfd, &rset);
              FD_SET(pipefd[0], &rset);

              if ( (n = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0 )
              {
                  if (errno == EINTR)
                  {
                       DEBUG;
                       printf("select errno = EINTR\n");    //+
                       continue;
                  }
                  else
                  {
                      err_sys("select error");
                  }
              }
//                printf("kankan!\n");
              if ( FD_ISSET(sockfd, &rset) )
              {
                   len = servlen;
                   n = my_recvfrom(sockfd, recvline, MAXLINE,
                                    0, preply_addr, &len);
                   recvline[n] = 0;
                   printf("\nfrom %s: %s",
                           my_sock_ntop(preply_addr, len),
                           recvline);
              }

              if ( FD_ISSET(pipefd[0], &rset) )
              {
//                   printf("read 上面\n");
                   my_read(pipefd[0], &n, 1);   //timer expired
//                   printf("read 下面!\n");
                   break;
              }
         }
    }

    free(preply_addr);

}

static void recvfrom_alarm(int signo)
{
    printf("时间到!\n");                    //+
//    printf("pipefd[1] = %d\n", pipefd[1]);  //+
    my_write(pipefd[1], "", 1);
return ;
}
