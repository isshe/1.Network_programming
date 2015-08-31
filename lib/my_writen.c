/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_writen.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.writen()
*                2.my_writen()
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;
            }
            else
            {
                return (-1);
            }
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }

    return (n);
}


void my_writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
    {
        err_sys("writen error");
    }
}
