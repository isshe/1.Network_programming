/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_tcp.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/19
* Description  : 使用getaddrinfo()等函数的几个包裹函数
* 				  用于简化访问getaddrinfo()的TCP接口函数
* Function list: 1.my_tcp_connect();p255
*                2.my_tcp_listen();p258
*                3.
* History      :
\*==============================================================================*/

#include "my.h"


//p255
//执行客户的通常步骤
int
tcp_connect(const char *host, const char *serv)
{
    int     sockfd = 0;
    int     n = 0;
    struct  addrinfo    hints;
    struct  addrinfo    *res = NULL;
    struct  addrinfo    *ressave = NULL;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;            // 这里影响返回的地址!!!
    hints.ai_socktype = SOCK_STREAM;        //TCP

    //n 如果不是0, 则代表一种错误, &res是链表
    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 )
    {
         err_quit("tcp_connect error for %s %s: %s",
                  host, serv, gai_strerror(n));
    }

    ressave = res;          //为什么?

    do
    {
        //系统调用
         sockfd = socket(res->ai_family,
                         res->ai_socktype,
                         res->ai_protocol);

         if (sockfd < 0)
         {
             continue;
         }

         //系统调用
         if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
         {
              break;            //success!
         }

         my_close(sockfd);

      //这里可能改变res, 所以需要ressave
    }while ((res = res->ai_next) != NULL);

    if (NULL == res)
    {
         err_sys("tcp_connect error for %s, %s", host, serv);
    }

    freeaddrinfo(ressave);

    return (sockfd);
}



//p258
//执行服务器的通常步骤
int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
     int    listenfd = 0;
     int    n = 0;
     const  int         on = 1;
     struct addrinfo    hints;
     struct addrinfo    *res = NULL;
     struct addrinfo    *ressave = NULL;

     bzero(&hints, sizeof(struct addrinfo));
     hints.ai_flags     = AI_PASSIVE;
     hints.ai_family    = AF_UNSPEC;
     hints.ai_socktype  = SOCK_STREAM;

     printf("listen中!!!");
     //if n != 0, 表示error
     if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 )
     {
          err_quit("tcp_listen error for %s, %s: %s",
                   host, serv, gai_strerror(n));
     }

     ressave = res;             //保存res

     do
     {
          //系统调用
          listenfd = socket(res->ai_family,
                            res->ai_socktype,
                            res->ai_protocol);

          if (listenfd < 0)
          {
               continue;        //error, try next one
          }

          my_setsockopt(listenfd, SOL_SOCKET,
                        SO_REUSEADDR,
                        &on, sizeof(on));

          //系统调用
          if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
          {
               break;       //success!
          }

          my_close(listenfd);
     }while( (res = res->ai_next) != NULL );

     if (res == NULL)
     {
          err_sys("tcp_listen error for %s, %s", host, serv);
     }

     my_listen(listenfd, LISTENQ);

     if (addrlenp)      //addrlenp是一个指针
     {
          //return size of protocal address(值结果参数)
          *addrlenp = res->ai_addrlen;
     }


          //printf("")
          struct sockaddr   test ;
          printf("ai_addr = %s\n", my_inet_ntop(AF_INET, (const char *)ressave->ai_addr, (char*)&test, sizeof(test)));


    freeaddrinfo(ressave);

     return (listenfd);
}
