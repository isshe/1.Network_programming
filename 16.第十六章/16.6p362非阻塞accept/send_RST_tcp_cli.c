/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : send_RST_tcp_cli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 建立连接并发送一个RST的TCP回射客户程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int main(int argc, char **argv)
{
     int    sockfd = 0;
     struct linger          ling ;
     struct sockaddr_in     servaddr;

     if (argc != 2)
     {
          err_quit("usage: ./a.out <IPaddress>");
     }

     sockfd = my_socket(AF_INET, SOCK_STREAM, 0);

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port   = htons(SERV_PORT);
     my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

     my_connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

     ling.l_onoff = 1;         //cause RST to be sent on close()
     ling.l_linger = 0;
     my_setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

     my_close(sockfd);

     exit(0);
}
