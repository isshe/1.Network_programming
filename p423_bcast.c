/*
 * =====================================================================================
 *
 *       Filename:  p423_bcast.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年07月31日 星期五 05時05分41秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *		 遇到问题：	dg_cli函数中的循环recvfrom()，只能接收到一条返回信息！
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <error.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>

#include "sock_ntop_host.h"			//为了函数sock_ntop_host()



#define SA struct sockaddr
#define MAXLINE 4096

static void recvfrom_alarm(int);
//static char* Fgets(char *sendline, int len, FILE *fp);
//static void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, 
//					socklen_t servlen);

int main(int argc, char **argv)
{
	int 				n = 0;
	int					sockfd = 0;;
	struct sockaddr_in	servaddr;

	if (argc != 2)
	{
		printf("usage: ./exe <IPaddress>\n");
		return 1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port	= htons(13);

	if ((n = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0)
	{
		printf("inet_pton error for %s\n", argv[1]);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket error!\n");
		exit(0);
	}

	dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	return 0;

}

static void recvfrom_alarm(int signo)
{
	printf("进入recvfrom_alarm函数！\n");
	return ;		//just interrupt the recvfrom()
}

//static
char* Fgets(char *sendline, int len, FILE *fp)
{
	char *temp = NULL;
	
	if ((temp = fgets(sendline, MAXLINE, fp)) == NULL && ferror(fp))
	{
		printf("fgets error! exit!\n");
		exit(1);
	}
	else
	{
		printf("fgets success!\n");
	}
	
	return temp;
}

//	static
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr,
					socklen_t servlen)
{
	int 					n = 0;
	const int				on = 1;
	char 					sendline[MAXLINE];
	char					recvline[MAXLINE];
	socklen_t				len = 0;
	struct sockaddr 		*preply_addr = NULL;
	char 					*rptr = NULL;

	if ((preply_addr = malloc(servlen)) == NULL)
	{
		printf("malloc error! exit!\n");
		exit (1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
	{
		printf("setsockopt error! exit!\n");
		exit(1);
	}

	if (signal(SIGALRM, recvfrom_alarm) == SIG_ERR)
	{
		printf("signal error! exit!\n");
		exit(1);
	}
	else	//以下三行貌似没什么用！
	{
		printf("signal success!\n");
	}

	
	printf("请输入：\n");
	while (Fgets(sendline, MAXLINE, fp) != NULL)
	{
		if (sendto(sockfd, sendline, (ssize_t)strlen(sendline),
					0, pservaddr, servlen) != strlen(sendline))
		{
			printf("sendto error!\n");
		}
		
		alarm(5);

		while (1)		//遇到问题，只能接收一条返回信息！！！
		{
			len = servlen;
			n = recvfrom(sockfd, recvline, MAXLINE,
						 	0, preply_addr, &len);

			if (n < 0)
			{
				if (errno == EINTR)
				{
					break;		//waited long enough for replies
				}
				else
				{
					printf("recvfrom error!\n");
					break;		//自己加的
				}
			}
			else
			{
				recvline[n] = 0;		//null terminate
				
				if ((rptr = sock_ntop_host(preply_addr, len)) == NULL)
				{
					printf("sock_ntop_host error!\n");
				}

				printf("from %s: %s", rptr, recvline);	//recvline本来就含有\n，所以可以不用！
				//break; 	//只有一台主机，加这个来重复输入！
			}
		}
	}

	free(preply_addr);

}
