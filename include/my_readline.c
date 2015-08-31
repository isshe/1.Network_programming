/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_readline.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.my_readline.c
*                2.
*                3.
* History      :
\*==============================================================================*/
#include "my.h"

static ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t     n ;
    ssize_t     rc;
    char        c ;
    char        *ptr;

    ptr = vptr;

    for (n = 1; n < maxlen; n++)
    {
again:
        if (( rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
            {
                break;
            }
        }
        else if (rc == 0)
        {
            *ptr = 0;
            return (n - 1);
        }
        else
        {
             if (errno == EINTR)
             {
                 goto again;
                 return (-1);
             }
        }
    }

    *ptr = 0;
    return ( n );
}

ssize_t my_readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t     n;

    if ((n = readline(fd, ptr, maxlen)) < 0)
    {
        err_sys("readline error");
    }

    return (n);
}
