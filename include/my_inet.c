/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_inet.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.my_inet_ntop()
*                2.my_inet_pton()
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

const char *my_inet_ntop(int family, const void *addrptr,
                         char *strptr, size_t len)
{
    const char *ptr;

    if (strptr == NULL)
    {
        err_quit("NULL 3rd argument to inet_ntop");
    }

    if ((ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
    {
        err_sys("inet_ntop error");
    }

    return (ptr);
}

void my_inet_pton(int family, const char *strptr, void *addrptr)
{
    int n = 0;

    if ((n = inet_pton(family, strptr, addrptr)) < 0)
    {
        err_sys("inet_pton error for %s", strptr);
    }
    else if (n == 0)
    {
        err_quit("inet_pton error for %s", strptr);
    }

    //nothing to return
}
