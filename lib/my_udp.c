/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_udp.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 用于简化访问getaddrinfo()的UDP接口函数
* Function list: 1.udp_client(); p262
*                2.udp_connect(); p264
*                3.udp_server(); p265
* History      :
\*==============================================================================*/


#include "my.h"

//p262
//创建一个未连接的UDP套接字
int udp_client(const char *host, const char *serv,
                SA **saptr, socklen_t *lenp)
{
    int     sockfd = 0;
    int     n = 0;
    struct  addrinfo    hints;
    struct  addrinfo    *res = NULL;
    struct  addrinfo    *ressave = NULL;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 )
    {
         err_quit("udp_client error for %s, %s: %s",
                  host, serv, gai_strerror(n));
    }

    ressave = res;      //用于freeaddrinfo()

    do
    {
         sockfd = socket(res->ai_family,
                         res->ai_socktype,
                         res->ai_protocol);

         if (sockfd >= 0)
         {
             break;
         }
    }while ( (res = res->ai_next) != NULL );

    if ( NULL == res )
    {
         err_sys("udp_client error for %s, %s", host, serv);
    }

    //通过值-结果参数返回
    *saptr = my_malloc(res->ai_addrlen);
    memcpy(*saptr, res->ai_addr, res->ai_addrlen);
    *lenp = res->ai_addrlen;

    freeaddrinfo(ressave);

    return 0;

}


//p264
//创建一个已连接的UDP套接字
int upd_connect(const char *host, const char *serv)
{
    int     sockfd = 0;
    int     n = 0;
    struct  addrinfo    hints;
    struct  addrinfo    *res = NULL;
    struct  addrinfo    *ressave = NULL;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 )
    {
         err_quit("udp_connect error for %s, %s: %s",
                  host, serv, gai_strerror(n));
    }

    ressave = res;

    do
    {
        sockfd = socket(res->ai_family,
                        res->ai_socktype,
                        res->ai_protocol);
        if (sockfd < 0)
        {
             continue;
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
        {
            break;
        }

        //connect 不成功的套接字描述符要关掉
        my_close(sockfd);
    }while ( (res = res->ai_next) != NULL );

    if (NULL == res)
    {
         err_sys("udp_connect error fof %s, %s", host, serv);
    }

    freeaddrinfo(ressave);

    return (sockfd);
}


//p265
//为UDP创建一个未连接的套接字
int udp_server(const char *host, const char *serv, socklen_t *addrlenp)
{
    int     sockfd = 0;
    int     n = 0;
    struct  addrinfo    hints ;
    struct  addrinfo    *res = NULL;
    struct  addrinfo    *ressave = NULL;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;            //注意这里
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 )
    {
         err_quit("udp_server error for %s, %s: %s",
                  host, serv, gai_strerror);
    }

    ressave = res;

    do
    {
         sockfd = socket(res->ai_family,
                         res->ai_socktype,
                         res->ai_protocol);
         if (sockfd < 0)
         {
             continue;
         }

         if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
         {
              break;
         }

         //bind失败的套接字也要关闭吗?
         my_close(sockfd);
    }while ( (res = res->ai_next) != NULL );

    if (NULL == res)
    {
        err_sys("udp_ser error for %s, %s", host, serv);
    }

    //返回
    if (addrlenp)
    {
         *addrlenp = res->ai_addrlen;
    }

    freeaddrinfo(ressave);

    return (sockfd);
}
