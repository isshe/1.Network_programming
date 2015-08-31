/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : family_to_level.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/27
* Description  :
* Function list: 1.family_to_level();
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"


/*==============================================================================\
* Function   (名称): family_to_level()
* Description(功能): 返回协议相对应的.......IPP....
* Called By  (被调): 1.mcast_join();
* Calls list (调用): 1.
* Input      (输入): 1.family;
* Output     (输出): 1.IPPROTO_IP; IPPROTO_IPV6
* Return     (返回):
*         success  :
*         error    : -1
* Others     (其他): 1.
\*==============================================================================*/
int family_to_level(int family)
{
     switch(family)
     {
         case AF_INET:
             return IPPROTO_IP;
#ifdef  IPV6
         case AF_INET6:
             return IPPROTO_IPV6;
#endif
         default:
             return -1;
     }
}

int my_family_to_level(int family)
{
     int    rc = 0;

     if ( (rc = family_to_level(family)) < 0 )
     {
         err_sys("family_to_level error");
     }

     return (rc);
}
