/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : udpcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/17
* Description  : UDP回射客户程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n = 0;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE + 1];              //!!!

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
         my_sendto(sockfd, sendline, strlen(sendline),
                    0, pservaddr, servlen);
         n = my_recvfrom(sockfd, recvline, MAXLINE,
                         0, NULL, NULL);

         recvline[n] = 0;

         my_fputs(recvline, stdout);
    }
}


int main(int argc, char **argv)
{
    int    sockfd = 0;
    struct sockaddr_in     servaddr;

    if (argc != 2)
    {
        err_quit("usage: ./udpcli <IPaddress>");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family  = AF_INET;
    servaddr.sin_port    = htons(SERV_PORT);
    my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
//    servaddr.sin_addr.s_addr = htonl(argv[1]);

    sockfd = my_socket(AF_INET, SOCK_DGRAM, 0);

    dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

    exit(0);

}


