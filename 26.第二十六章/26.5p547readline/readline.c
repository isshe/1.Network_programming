/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : readline.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/30
* Description  : 线程安全的readline函数
* Function list: 1.readline_destructor();
*                2.readline_once();
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

static pthread_key_t    rl_key;
static pthread_once_t   rl_once = PTHREAD_ONCE_INIT;

static void readline_destructor(void *ptr)
{
     free(ptr);
}

static void readline_once(void)
{
     my_pthread_key_create(&rl_key, readline_destructor);   //注意这里
}

typedef struct
{
     int    rl_cnt;             //initialize to 0
     char   *rl_bufptr;         //initialize to rl_buf
     char   rl_buf[MAXLINE];
}Rline;

static ssize_t  my_read(Rline *tsd, int fd, char *ptr)
{
     if (tsd->rl_cnt <= 0)
     {
again:
         if ( (tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0 )
         {
              if (errno == EINTR)
              {
                  goto again;
              }

              return (-1);
         }
         else if (tsd->rl_cnt == 0)
         {
              return 0;
         }

         tsd->rl_bufptr = tsd->rl_buf;      //这里不懂
     }

     tsd->rl_cnt--;
     *ptr = *tsd->rl_bufptr++;

     return (1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
     size_t     n ;
     size_t     rc;
     char       c ;
     char       *ptr = NULL;
     Rline      *tsd = NULL;

     my_pthread_once(&rl_once, readline_once);      //初始化

     if ( (tsd = pthread_getspecific(rl_key)) == NULL )
     {
          tsd = my_calloc(1, sizeof(Rline));        //init to 0;
          my_pthread_setspecific(rl_key, tsd);
     }

     ptr = vptr;

     for (n = 1; n < maxlen; n++)
     {
          if ( (rc = my_read(tsd, fd, &c)) == 1 )
          {
              *ptr++ = c;
              if ( c == '\n' )
              {
                   break;
              }
          }
          else if (rc == 0)
          {
              *ptr = 0;
              return (n - 1);       //EOF, n-1 bytes read
          }
          else
          {
               return (-1);         //error, errno set by read()
          }
     }

     *ptr = 0;
     return (n);
}
