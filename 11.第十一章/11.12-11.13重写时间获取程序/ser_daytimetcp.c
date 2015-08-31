/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : ser_daytimetcp.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/20
* Description  : 用tcp_listen()重新编写的时间获取服务器程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int main(int argc, char **argv)
{
     int    listenfd = 0;
     int    connfd = 0;
     char   buff[MAXLINE] = {0};
     time_t ticks ;
     struct sockaddr_in     cliaddr;
     socklen_t  len;

     if (argc != 2)
     {
          err_quit("usage: ./a.out <service or port#>");
     }

     listenfd = my_tcp_listen(NULL, argv[1], NULL);

     while (1)
     {
          len = sizeof(cliaddr);
          connfd = my_accept(listenfd, (SA *) &cliaddr, &len);
          printf("connect from %s\n", my_sock_ntop((SA *)&cliaddr, len));

          ticks = time(NULL);           //???
          snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
          my_write(connfd, buff, strlen(buff));

          my_close(connfd);
     }
}
