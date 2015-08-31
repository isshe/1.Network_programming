/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : daytimetcpcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/19
* Description  : 使用gethostbyname()和getservbyname()的时间获取客户程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int
main(int argc, char **argv)
{
    int     sockfd = 0;
    int     n = 0;
    char    recvline[MAXLINE + 1];
    struct  sockaddr_in     servaddr;
    struct  in_addr         **pptr = NULL;             //
    struct  in_addr         *inetaddrp[2];      //
    struct  in_addr         inetaddr;           //
    struct  hostent         *hp = NULL;
    struct  servent         *sp = NULL;

    if ( argc != 3 )
    {
         err_quit("usage: daytimetcpcli <hostname> <service>");
    }

    if ( (hp = gethostbyname(argv[1])) == NULL )
    {
        if (inet_aton(argv[1], &inetaddr) == 0)
        {
             err_quit("hostname error for %s: %s\n",
                      argv[1], hstrerror(h_errno));
        }
        else
        {
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;           //注意是有p的.
        }
    }
    else
    {
         pptr = (struct in_addr **) hp->h_addr_list;
    }

    //获得了相应服务的端口号什么的
    if ( (sp = getservbyname(argv[2], "tcp")) == NULL )
    {
        err_quit("getservbyname error for %s\n", argv[2]);
    }

    for ( ; *pptr != NULL; pptr++ )
    {
         sockfd = my_socket(AF_INET, SOCK_STREAM, 0);

         bzero(&servaddr, sizeof(servaddr));
         servaddr.sin_family    = AF_INET;
         servaddr.sin_port      = sp->s_port;       //帅
         memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
         printf("trying %s\n",
                 my_sock_ntop((SA *) &servaddr, sizeof(servaddr)));

         if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0)
         {
              break;        //success
         }
         err_ret("connect error");
         close(sockfd);
    }

    if (*pptr == NULL)                  //这里不是很懂
    {
         err_quit("unable to connect");
    }

    while ((n= my_read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        my_fputs(recvline, stdout);
    }

    exit (0);
}
