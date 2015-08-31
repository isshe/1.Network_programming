/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : udpser.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/17
* Description  :  p188页8.3到8.4.udp回射服务程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
     int    n = 0;
     socklen_t len ;
     char msg[MAXLINE];

     while (1)
     {
          len = clilen;
          n = my_recvfrom(sockfd, msg, MAXLINE,
          					 0, pcliaddr, &len);         
//          printf("recvfrom %s : %s\n",
//				 my_sock_ntop(pcliaddr, len), msg);

          my_sendto(sockfd, msg, n, 0, pcliaddr, len);
//          printf("sendto %s : %s\n",
//				 my_sock_ntop(pcliaddr, len), msg);
     }
}

//图8-20
static void recvfrom_int(int);
static int 	count;

void tu8_20dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	socklen_t 	len;
	char 		msg[MAXLINE];
	
	my_signal(SIGINT, recvfrom_int);
	//signal(SIGINT, recvfrom_int);
	while(1)
	{
		len = clilen;
		my_recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len);
		
		count++;
		printf("count = %d\n", count);
	}
}

static void recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}

int main(int argc, char **argv)
{
    int     sockfd = 0;
    struct  sockaddr_in     servaddr;
    struct  sockaddr_in     cliaddr;

    sockfd = my_socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);

    my_bind(sockfd, (SA *)&servaddr, sizeof(servaddr));

    tu8_20dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
    return 0;
}



