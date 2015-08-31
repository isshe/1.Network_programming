/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : poll_str_cli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 使用/dev/poll的str_cli函数
* Function list: 1.str_cli();
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

void str_cli(FILE *fp, int sockfd)
{
     int    stdineof = 0;
     char   buf[MAXLINE] = {0};
     int    n = 0;
     int    wfd = 0;
     int    i = 0;
     int    result = 0;
     struct pollfd  pollfd[2];
     struct dvpoll  dopoll ;            //这个.....

     wfd = my_open("dev/poll", O_RDWR, 0);

     pollfd[0].fd = fileno(fp);
     pollfd[0].events = POLLIN;
     pollfd[0].revents = 0;

     pollfd[1].fd = sockfd;
     pollfd[1].events = POLLIN;
     pollfd[1].revents = 0;

    my_write(wfd, pollfd, sizeof(struct pollfd) *2);

    stdineof = 0;           //???

    while(1)
    {
         //block until /dev/poll says something is ready
         dopoll.dp_timeout = -1;        //-1是阻塞, 0是非阻塞, >0是时间
         dopoll.dp_nfds = 2;
         dopoll.dp_fds = pollfd;
         result = my_ioctl(wfd, DP_POLL, &dopoll);

         //loop through ready file descriptors
        for (i = 0; i < result; i++)
        {
             if (dopoll.dp_fds[i].fd == sockfd)
             {
                 //socket is readable
                 if ( (n = my_read(sockfd, buf, MAXLINE)) == 0 )
                 {
                      if (stdineof == 1)
                      {
                          return ;          //normal termination
                      }
                      else
                      {
                           err_quit("str_cli: server terminated prematurely");
                      }
                 }

                 my_writ(file(stdout), buf, n);

             }
             else
             {
                  //input is readable
                  if ( (n = my_read(fileno(fp), buf, MAXLINE)) == 0 )
                  {
                      stdineof = 1;
                      my_shutdown(sockfd, SHUT_WR);     //send FIN
                      continue;
                  }

                  my_writen(sockfd, buf, n);
             }
        }
    }
}
