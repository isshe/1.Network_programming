/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sockfd_to_family.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/27
* Description  : 用sockfd获取family(协议族)
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

int sockfd_to_family(int sockfd)
{
    struct  sockaddr_storage ss;
    socklen_t   len;

    len = sizeof(ss);

    if (getsockname(sockfd, (SA *) &ss, &len) < 0)
    {
         return (-1);
    }

    return (ss.ss_family);
}

int my_sockfd_to_family(int sockfd)
{
     int    rc = 0;

     if ( (rc = sockfd_to_family(sockfd)) < 0 )
     {
         err_sys("sockfd_to_family error");
     }

     return (rc);
}
