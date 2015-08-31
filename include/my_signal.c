/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_signal.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.my_signal()
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

//加了这个导致信号方面的没有用,所以注释掉, 懂了再加上.
/*
Sigfunc* signal(int signo, Sigfunc *func)
{
    struct  sigaction act;
    struct  sigaction oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo = SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;       //SunOS 4.x
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0)
    {
        return (SIG_ERR);
    }

    return (oact.sa_handler);
}
*/
Sigfunc * my_signal(int signo, Sigfunc *func)
{
	Sigfunc *sigfunc;

	if ((sigfunc = signal(signo, func)) == SIG_ERR)
	{
		err_sys("signal error");
	}

	return sigfunc;
}


void my_sigaddset(sigset_t *set, int signo)
{
    if (sigaddset(set, signo) == -1)
    {
        err_sys("sigaddset error");
    }
}

void my_sigdelset(sigset_t *set, int signo)
{
    if (sigdelset(set, signo) == -1)
    {
        err_sys("sigdelset error");
    }
}

void my_sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) == -1)
    {
         err_sys("sigemptyset error");
    }
}

void my_sigfillset(sigset_t *set)
{
    if (sigfillset(set) == -1)
    {
        err_sys("sigfillset error");
    }
}


int my_sigismember(const sigset_t *set, int signo)
{
     int    n = 0;

     if ( (n = sigismember(set, signo)) == -1 )
     {
         err_sys("sigisemptyset error");
     }

     return (n);
}

void my_sigpending(sigset_t *set)
{
     if (sigpending(set) == -1)
     {
         err_sys("sigpending error");
     }
}

void my_sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
     if (sigprocmask(how, set, oset) == -1)
     {
         err_sys("sigprocmask error");
     }
}


