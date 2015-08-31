/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : ser_udp_mcast.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/27
* Description  : 多播服务器, 回射, 时间获取, mac地址等
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"
#include "ser_udp_mcast.h"

int main(void)
{
     int    sockfd = 0;
     struct sockaddr_in     servaddr;
     struct sockaddr_in     grpaddr;
     struct sockaddr_in     cliaddr;
     const  int on = 1;                 //用于开启选项

     sockfd = my_socket(AF_INET, SOCK_DGRAM, 0);

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family        = AF_INET;
     servaddr.sin_addr.s_addr   = htonl(INADDR_ANY);
     servaddr.sin_port          = htons(SERV_PORT);
     
     //SO_REUSEADDR的使用还不懂!对TCP, UDP各会如何, 不懂.
     my_setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
     my_bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

     bzero(&grpaddr, sizeof(servaddr));
     grpaddr.sin_family     = AF_INET;
     grpaddr.sin_addr.s_addr = inet_addr("224.0.0.8");

     mcast_join(sockfd, (SA *)&grpaddr, sizeof(grpaddr), NULL, 0);

     //回射发来的信息, 以及时间, mac地址
     dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
    
     return 0;
}
