/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpserv01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/12
* Description  : TCP回射服务器程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

//#define SERV_PORT 8888
//#define LISTENQ 1024

void str_echo(int sockfd)
{
    ssize_t     n ;
    char        buf[MAXLINE];


again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
    {
        my_write(sockfd, buf, n);
    }

    if (n < 0 && errno == EINTR)
    {
        goto again;
    }
    else if (n < 0)
    {
        err_sys("str_echo: read error");
    }
}

void tu5_17str_echo(int sockfd)
{
	long arg1 = 0;
	long arg2 = 0;
	ssize_t n;
	char line[MAXLINE];
	
	while (1)
	{
		if ( (n = my_readline(sockfd, line, MAXLINE)) == 0)
		{
			return ;
		}
		if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2)
		{
			snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
		}
		else
		{
			snprintf(line, sizeof(line), "input error\n");
		}
		
		n = strlen(line);
		my_writen(sockfd, line, n);
	}
}
/////////////////////下面两个信号处理函数/////////////////////
void tu5_7sig_chld(int signo) 		//不够正确版本
{
	pid_t 	pid;
	int 	stat;
	
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	
	return ;
}

void tu5_11sig_chld(int signo)
{
	pid_t 	pid;
	int 	stat;
	
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("child %d terminated\n", pid);
	}
	
	return ;
}

int main(int argc, char **argv)
{
    int     listenfd = 0;
    int     connfd = 0;
    pid_t   childpid ;
    socklen_t   clilen;
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;

    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);
	
	signal(SIGCHLD, tu5_11sig_chld);						///
	
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    my_bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    my_listen(listenfd, LISTENQ);

    while(1)
    {
        clilen = sizeof(cliaddr);
        if ( (connfd = my_accept(listenfd, (SA*)&cliaddr, &clilen)) < 0)
        {
        
        }

        if ((childpid = my_fork()) == 0)
        {
            my_close(listenfd);
            tu5_17str_echo(connfd);
            exit(0);
        }
        my_close(connfd);
    }

    return 0;
}

