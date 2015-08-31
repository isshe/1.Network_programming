/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : inet_pton_loose.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/12
* Description  : 如果为AF_INET&inet_pton()返回0,那就调用inet_aton看是否成功;
*                如果为AF_INET&inet_pton()返回0, 那就调用inet_aton看是否成功,
*                成功则返回其IPv4映射的IPv6地址;
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


int inet_pton_loose(int family,
                    char *strptr,
                    void *addrptr)
{
    struct in_addr  in_val;
    int repton = 0;
    char buf[64] = {0};


    if(AF_INET == family)
    {
        if((repton = inet_pton(AF_INET, strptr, addrptr)) == 0)
        {
            if (inet_aton(strptr, &in_val))
            {
                printf("inet_aton4 limian\n");
                memcpy(addrptr, &in_val, sizeof(struct in_addr));
                return 1;
            }
            else
                printf("AF_INET inet_aton != 1\n");
        }
        else
            printf("AF_INET inet_pton != 0!\nrepton: %d\n", repton);
    }
    else if (AF_INET6 == family)
    {

        if ((repton = inet_pton(AF_INET6, strptr, addrptr)) == 0)
        {
            if (inet_aton(strptr, &in_val))
            {
                memcpy(addrptr, &in_val, sizeof(struct in_addr));
                snprintf(buf, 64, "0:0:0:0:0:FFFF:%s", strptr);
                memcpy(strptr, buf, sizeof(buf));
                return 1;
            }
        }
        else
            printf("AF_INET6 inet_pton != 0!\nrepton: %d\n", repton);
    }

    return 0;
}

int main(int argc, char **argv)
{
    char strptr[64] = {0};
    struct  sockaddr_in servaddr;
    struct sockaddr_in6 servaddr6;


    if (argc != 2)
    {
        printf("usage: ./a.out <IPaddress>\n");
        return 0;
    }

//    printf("memcpy argv!\nstrptr = %s\n", strptr);
    memcpy(strptr, argv[1], strlen(argv[1]) + 1);

    if (inet_pton_loose(AF_INET6, strptr, &servaddr6.sin6_addr))
    {
        printf("inet_pton_loose6 success!\n");
        printf("strptr = %s\n", strptr);
    }
    else
    {
        printf("inet_pton_loose6 error!\n");
    }


    memcpy(strptr, argv[1], strlen(argv[1]) + 1);
    if (inet_pton_loose(AF_INET, strptr, &servaddr.sin_addr))
    {
        printf("inet_pton_loose4 success!\n");
//        printf("strptr = %s\n", strptr);
    }
    else
    {
        printf("inet_pton_loose4 error!\n");
    }


    return 0;
}
