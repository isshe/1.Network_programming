/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : example02_right.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/30
* Description  : 使用互斥锁保护共享变量的图26-7的修正版
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

#define NLOOP 5000

int counter;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void    *doit(void *);

int main(int argc, char **argv)
{
     pthread_t  tidA;
     pthread_t  tidB;

     my_pthread_create(&tidA, NULL, &doit, NULL);
     my_pthread_create(&tidB, NULL, &doit, NULL);

     //wait for both threads to terminate
     my_pthread_join(tidA, NULL);
     my_pthread_join(tidB, NULL);

     exit(0);
}

void *doit(void *vptr)
{
     int    i = 0;
     int    val = 0;

     for (i = 0; i < NLOOP; i++)
     {
         my_pthread_mutex_lock(&counter_mutex);

         val = counter;
         printf("%d: %d\n",
                 pthread_self(), val + 1);
         counter = val + 1;

         my_pthread_mutex_unlock(&counter_mutex);
     }

     return (NULL);
}
