/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : cli_daytimetcp.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/20
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

int main(int argc, char **argv)
{
     int    sockfd = 0;
     int    n = 0;
     char   recvline[MAXLINE] = {0};
     struct sockaddr_storage    ss ;        //
     socklen_t  len;

     if (argc != 3)
     {
          err_quit("usage: ./a.out <hostname/IPaddress> <service/port#>");
     }

     sockfd = tcp_connect(argv[1], argv[2]);

     len = sizeof(ss);

     my_getpeername(sockfd, (SA *) &ss, &len);

     printf("connect to %s\n", my_sock_ntop((SA *) &ss, len));

     while ( (n = my_read(sockfd, recvline, MAXLINE)) > 0 )
     {
          recvline[n] = 0;
          my_fputs(recvline, stdout);
     }

     exit (0);
}
