/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sockfd_to_family.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/12
* Description  : 用getsockname()获取套接字的地址族
* Function list: 1.sockfd_to_family();
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "../../include/my.h"

int sockfd_to_family(int sockfd)
{
    struct  sockaddr_storage ss;
    socklen_t   len;

    len = sizeof(ss);
    if (getsockname(sockfd, (SA *)&ss, &len) < 0)
    {
        return -1;
    }

    return (ss.ss_family);
}


int main(int argc, char **argv)
{
    int sockfd = 0;
    struct sockaddr_in  servaddr;
    struct sockaddr_storage ress;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_sys("socket error");
    }

    ress.ss_family = sockfd_to_family(sockfd);

    printf("ss_family = %d\n", ress.ss_family);

    return 0;
}
