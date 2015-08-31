/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : connect_nonb.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 发起一个非阻塞connect;
* Function list: 1.connect_nonb()
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

int connect_nonb(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
    int     flags = 0;
    int     n = 0;
    int     error = 0;
    fd_set  rset;
    fd_set  wset;
    struct  timeval     tval;
    socklen_t   len;

    //设置非阻塞
    flags = my_fcntl(sockfd, F_GETFL, 0);
    my_fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0;
    
    //以下这块自己加的
    len = sizeof(error);       //???
    if ((n = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len)) < 0)
    {
    	printf("getsockopt < 0\n");
        return (-1);
    }
    else if ( n == 0)
    {
    	printf("getsockopt = 0\n");
    }   
    else
    {
    	printf("getsockopt > 0\n");
    }
    

    if ( (n = connect(sockfd, saptr, salen)) < 0 )
    {
    	 printf("connect return < 0! error: %s\n", strerror(errno));
         if (errno != EINPROGRESS)
         {
             return (-1);
         }
    }




    //Do whatever we want while the connect is taking palce

    if (0 == n)
    {
    	 printf("connect return 0!\n非阻塞connect可能没有成功\n");
         goto done;         //connect completed immediately
    }

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;            //
    tval.tv_sec = nsec;
    tval.tv_usec = 0;

    if ( (n = my_select(sockfd + 1, &rset, &wset, NULL,
                        nsec ? &tval : NULL)) == 0 )
    {
        close(sockfd);
        errno = ETIMEDOUT;
        return (-1);
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset))
    {
         len = sizeof(error);       //???
         if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
         {
             return (-1);
         }
    }
    else
    {
         err_quit("select error: sockfd not set");
    }

done:
    my_fcntl(sockfd, F_SETFL, flags);   //restore file status flags /何用?

    if ( error )
    {
         close(sockfd);         //just in case
         errno = error;
         return (-1);
    }

    return 0;
}
