/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpcli01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.main()
*                2.str_cli()
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "tcpcli01.h"

/* 1. */
static void str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
        my_writen(sockfd, sendline, strlen(sendline));

        if (my_readline(sockfd, recvline, MAXLINE) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }

        my_fputs(recvline, stdout);
    }
}

/* 2. */
static void tu5_14str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
        my_writen(sockfd, sendline, 1);
		sleep(1);		
		my_writen(sockfd, sendline + 1, strlen(sendline) - 1);
		
        if (my_readline(sockfd, recvline, MAXLINE) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }

        my_fputs(recvline, stdout);
    }
}



int main(int argc, char **argv)
{
//    int sockfd[5] = {0};
	int sockfd = 0;
    int i = 0;
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        err_quit("usage: ./tcpcli <IPaddress>");
    }
	
//	for (i = 0; i < 5; i++)
//	{
//		sockfd[i] = my_socket(AF_INET, SOCK_STREAM, 0);
		sockfd = my_socket(AF_INET, SOCK_STREAM, 0);
	
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);
		my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

//		my_connect(sockfd[i], (SA *)&servaddr, sizeof(servaddr));
		my_connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
//	}
//    tu5_14str_cli(stdin, sockfd[0]);
    tu5_14str_cli(stdin, sockfd);

    exit (0);
}
