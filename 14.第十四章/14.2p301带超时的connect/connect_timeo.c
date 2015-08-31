/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : connect_timeo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 带超时的connect
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

static void connect_alarm(int);

int connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
     Sigfunc *sigfunc;
    int     n = 0;

    sigfunc = my_signal(SIGALRM, connect_alarm);

    //成功:如果先前已经设置过alarm(),
    //     则alarm()返回上次剩余的秒数, 否则返回0
    //失败: -1
    if (alarm(nsec) != 0)
    {
         err_msg("connect_timeo: alarm was already set");
    }

    if ( (n =  connect(sockfd, saptr, salen)) < 0 )
    {
         close(sockfd);
         if (errno == EINTR)
         {
             errno = ETIMEDOUT;
         }
    }

    alarm(0);                       //turn off the alarm
    my_signal(SIGALRM, sigfunc);    //restore previous signal handle

    return (n);
}

static void
connect_alarm(int signo)
{
    return ;                //just interrupt the connect
}
