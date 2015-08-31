/*==============================================================================*\
* Copyright(C)2015 Chudai.
*
* File name    : gd_echo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/27
* Description  : 回射发来的信息, 送时间, 本机mac!
*                   注意: 获取mac地址时, 是固定了接口号"eth0"!!!
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

//图8-20
static void recvfrom_int(int);
static int 	count;

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	socklen_t 	len;
	char 		msg[MAXLINE];
//	int         n = 0;
	char        buf[MAXLINE + 64] ;
	time_t      ticks;                  //用于获取时间
	struct      ifreq   ifr ;    //用于获取mac地址
	u_char        *hwaddr = NULL;          //用于指向获取的mac地址
	
	my_signal(SIGINT, recvfrom_int);
	//signal(SIGINT, recvfrom_int);
	while(1)
	{
		len = clilen;
//		n = 
		my_recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len);
		
		count++;
		printf("%d. recvfrom %s: %s\n", 
		        count, 
		        my_sock_ntop(pcliaddr, len), 
		        msg);
		//获取时间, 然后获取mac地址, 然后回射
		//获取时间:
		ticks = time(NULL);
		
		//获取mac地址
		//这句!!!疑问: 要怎么改才能动态获取呢?否则如果不是eth0就会失败.
		//或许, 在运行程序前,让用户指定接口?!
		memcpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);  
		my_ioctl(sockfd, SIOCGIFHWADDR, &ifr);
		hwaddr = (u_char *)ifr.ifr_hwaddr.sa_data;//ifr_hwaddr是一个"SA"结构
		
		snprintf(buf, MAXLINE + 64, 
		         "%s\r"
		         "     %.24s\r\n"
		         "     hwaddr=%x:%x:%x:%x:%x:%x\r\n",
		         msg, 
		         ctime(&ticks),
		         *(hwaddr), *(hwaddr + 1), *(hwaddr + 2),
		         *(hwaddr + 3), *(hwaddr + 4), *(hwaddr + 5));
		printf("%s\n", buf);
		my_sendto(sockfd, buf, sizeof(buf), 0, pcliaddr, len);
	}
}

static void recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}

