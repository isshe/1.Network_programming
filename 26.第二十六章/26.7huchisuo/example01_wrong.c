/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : example01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/30
* Description  : 两个线程不正确地递增一个全局变量
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

#define NLOOP   5000

int     counter;        //incremented by threads

void *doit(void *);

int main(int argc, char **argv)
{
     pthread_t  tidA;
     pthread_t  tidB;

     my_pthread_create(&tidA, NULL, &doit, NULL);
     my_pthread_create(&tidB, NULL, &doit, NULL);

     //wait for both threads to terminate
     my_pthread_join(tidA, NULL);
     my_pthread_join(tidB, NULL);

     return 0;
}

void *doit(void *vptr)
{
     int    i = 0;
     int    val = 0;

     for (i = 0; i < NLOOP; i++)
     {
          val = counter;
          printf("%d: %d\n",
                  pthread_self(), val + 1);
          counter = val + 1;
     }

     return (NULL);
}

