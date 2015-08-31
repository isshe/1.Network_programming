/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : daytimetcpcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/11
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "../../include/my.h"

int main(int argc, char **argv)
{
    int     sockfd = 0;
    int     n = 0;
    char    recvline[MAXLINE + 1];
    long    count = 0;
    struct  sockaddr_in    servaddr;


    if (argc != 2)
    {
        err_quit("usage: a.out <IPaddress>");
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket errno = %d\n", errno);
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family     = AF_INET;
    servaddr.sin_port       = htons(8888);	//是13是系统的,8888是自己定的

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
    {
        err_sys("connect error");
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        count++;

        recvline[n] = 0;

        if (fputs(recvline, stdout) == EOF)
        {
            err_sys("fputs error");
        }
    }

    if (n < 0)
    {
         err_sys("read error");
    }

    printf("count = %ld\n", count);

    return 0;
}
