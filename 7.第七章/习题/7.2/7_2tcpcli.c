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
//#include <tcp.h>
//#define TCP_MAXSEG 1460
#include <netinet/tcp.h> 		//for TCP_MAXSEG

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



int main(int argc, char **argv)
{
    int sockfd = 0;
    int optval = 0;
    socklen_t optlen ;
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        err_quit("usage: ./tcpcli <IPaddress>");
    }

    sockfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	//recvbuf
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("connect qian: recvbuf = %d\n", optval);
	//MSS
	getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &optval, &optlen);
	printf("connect qian: MSS = %d\n", optval);
	
    my_connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    
    //recvbuf
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("connect hou: recvbuf = %d\n", optval);
	//MSS
	getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &optval, &optlen);
	printf("connect hou: MSS = %d\n", optval);

    str_cli(stdin, sockfd);

    return 0;
}
