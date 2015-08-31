/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : udpcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/17
* Description  : 用了connect的UDP程序
*              : 连接外地IP, 然后获取本地IP,打印.
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int
main (int argc, char **argv)
{
    int     sockfd;
    socklen_t   len ;
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;

    if (argc != 2)
    {
         err_quit("usage: ./udpcli <IPaddress>");
    }

    sockfd = my_socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    my_connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    len = sizeof(cliaddr);
    my_getsockname(sockfd, (SA *) &cliaddr, &len);
    printf("local address %s\n", my_sock_ntop((SA *) &cliaddr, len));

    exit ( 0 );
}
