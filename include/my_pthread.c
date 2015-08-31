/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_pthread.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/28
* Description  :
* Function list: 1.  my_pthread_create();
*                2.  my_pthread_join();
*                3.  my_pthread_detach();
*                4.  my_pthread_kill();
*                5.  my_pthread_mutexattr_init();
*                6.  my_pthread_mytexattr_setpshared();
*                7.  my_pthread_mutex_init();
*                8.  my_pthread_mutex_lock();
*                9.  my_pthread_mutex_unlock();
*                10. my_pthread_cond_broadcast();
*                11. my_pthread_cond_signal();
*                12. my_pthread_cond_wait();
*                13. my_pthread_cond_timewait();
*                14. my_pthread_once();
*                15. my_pthread_key_create();
*                16. my_pthread_setspecific();
* History      :
\*==============================================================================*/

#include "my.h"

void my_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                       void *(*func)(void *), void *arg)
{
     int    n = 0;
     if ( (n = pthread_create(tid, attr, func, arg)) == 0 )
     {
         return ;
     }

     errno = n;
     err_sys("pthread_create error");
}

void my_pthread_join(pthread_t tid, void **status)
{
     int    n = 0;
     if ( (n = pthread_join(tid, status)) ==  0)
     {
         return ;
     }

     errno = n;
     err_sys("pthread_join error");
}

void my_pthread_detach(pthread_t tid)
{
     int    n = 0;
     if ( (n = pthread_detach(tid)) == 0 )
     {
         return ;
     }

     errno = n;
     err_sys("pthread_detach error");
}

void my_pthread_kill(pthread_t tid, int signo)
{
     int    n = 0;

     if ( (n = pthread_kill(tid, signo)) == 0 )
     {
         return ;
     }

     errno = n;
     err_sys("pthread_kill error");
}


void my_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
     int    n = 0;
     if ( (n = pthread_mutexattr_init(attr)) == 0 )
     {
         return ;
     }

     errno = n;
     err_sys("pthread_mutexattr_init error");
}

#ifdef  _POSIX_THREAD_PROCESS_SHARED
void my_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int flag)
{
     int    n = 0;
     if ( (n = pthread_mutexattr_setpshared(attr, flag)) == 0 )
     {
         return;
     }

     errno = n;
     err_sys("pthread_mutexattr_setpshared error");
}

#endif


void
my_pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr)
{
	int		n;

	if ( (n = pthread_mutex_init(mptr, attr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_init error");
}

/* include Pthread_mutex_lock */
void
my_pthread_mutex_lock(pthread_mutex_t *mptr)
{
	int		n;

	if ( (n = pthread_mutex_lock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_lock error");
}
/* end Pthread_mutex_lock */

void
my_pthread_mutex_unlock(pthread_mutex_t *mptr)
{
	int		n;

	if ( (n = pthread_mutex_unlock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_unlock error");
}

void
my_pthread_cond_broadcast(pthread_cond_t *cptr)
{
	int		n;

	if ( (n = pthread_cond_broadcast(cptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_broadcast error");
}

void
my_pthread_cond_signal(pthread_cond_t *cptr)
{
	int		n;

	if ( (n = pthread_cond_signal(cptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_signal error");
}

void
my_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
	int		n;

	if ( (n = pthread_cond_wait(cptr, mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_wait error");
}

void
my_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
					   const struct timespec *tsptr)
{
	int		n;

	if ( (n = pthread_cond_timedwait(cptr, mptr, tsptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_timedwait error");
}

void
my_pthread_once(pthread_once_t *ptr, void (*func)(void))
{
	int		n;

	if ( (n = pthread_once(ptr, func)) == 0)
		return;
	errno = n;
	err_sys("pthread_once error");
}

void
my_pthread_key_create(pthread_key_t *key, void (*func)(void *))
{
	int		n;

	if ( (n = pthread_key_create(key, func)) == 0)
		return;
	errno = n;
	err_sys("pthread_key_create error");
}

void
my_pthread_setspecific(pthread_key_t key, const void *value)
{
	int		n;

	if ( (n = pthread_setspecific(key, value)) == 0)
		return;
	errno = n;
	err_sys("pthread_setspecific error");
}
