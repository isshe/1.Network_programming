/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : mcast_get_set_loop.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/27
* Description  : 设置/获取多播回馈选项
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"


/*==============================================================================\
* Function   (名称): mcast_set_loop();
* Description(功能): 设置多播回馈选项
* Called By  (被调): 1.
* Calls list (调用): 1.
* Input      (输入): 1.sockfd; 2.onoff; 3.
* Output     (输出): 1.
* Return     (返回):
*         success  :
*         error    : -1
* Others     (其他): 1.
\*==============================================================================*/
int mcast_set_loop(int sockfd, int onoff)
{
     switch (my_sockfd_to_family(sockfd))
     {
         case AF_INET:
             {
                 u_char     flag;

                 flag = onoff;
                 return (setsockopt(sockfd, IPPROTO_IP,
                                    IP_MULTICAST_LOOP,
                                    &flag, sizeof(flag)));
             }

#ifdef  IPV6
         case AF_INET6:
             {
                  u_int     flag;
                  flag = onoff;

                  return (setsockopt(sockfd, IPPROTO_IPV6,
                                     IPV6_MULTICAST_LOOP,
                                     &flag, sizeof(flag)));
             }
#endif
         default:
             errno = EAFNOSUPPORT;
             DEBUG;
             printf("#errno = EAFNOSUPPORT");

             return (-1);
     }


}
