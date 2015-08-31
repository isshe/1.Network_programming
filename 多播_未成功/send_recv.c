/*
 * =====================================================================================
 *
 *       Filename:  send_recv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 11時57分51秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "my.h"
#include "test_main.h"


void send_all(int sendfd, SA *sadest, socklen_t salen)
{
	char line[MAXLINE];
	struct utsname myname;
	
	if (uname(&myname) < 0)
	{
		err_sys("uname error");
	}
	
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());
	
	while(1)
	{
		my_sendto(sendfd, line, strlen(line), 0, sadest, salen);
		sleep(SENDRATE);
	}
}

void recv_all(int recvfd, socklen_t salen)
{
	int n = 0;
	char line[MAXLINE + 1];
	socklen_t len;
	struct sockaddr *safrom;
	
	safrom = my_malloc(salen);
	
	while(1)
	{
		len = salen;
		n = my_recvfrom(recvfd, line, MAXLINE, 0, safrom, &len);
		line[n] = 0;
		printf("from %s:%s", my_sock_ntop(safrom, len), line);
	}
}

