/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpcli01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.my_select();
*                2.my_poll();
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

//#ifdef	HAVE_POLL
int my_poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int		n;

	if ( (n = poll(fdarray, nfds, timeout)) < 0)
		err_sys("poll error");

	return(n);
}
//#endif

int my_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		err_sys("select error");
	return(n);		/* can return 0 on timeout */
}

