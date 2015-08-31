/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : daytimetcpsrv.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/11
* Description  : TCP时间获取函数服务器,与1.2的程序一起工作
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "../../include/my.h"
#include <time.h>

int main(int argc, char **argv)
{
    int     listenfd = 0;
    int     connfd = 0;
    char    buff[MAXLINE] = {0};
    time_t  ticks ;
    socklen_t 	len ;
    struct  sockaddr_in     servaddr;
    struct 	sockaddr_in 	cliaddr;

    listenfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(8888);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    my_bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    my_listen(listenfd, 1024);

    while(1)
    {
        printf("正阻塞在accept(),等待连接!\n");
        connfd = my_accept(listenfd, (SA *)&cliaddr, &len);     //注意这里!!!!
        printf("connection form %s, port %d\n",
        		inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
        		ntohs(cliaddr.sin_port));
        
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));

        close(connfd);
    }
}
