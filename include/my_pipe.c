/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_pipe.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/26
* Description  :
* Function list: 1.my_pipe();
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

/*==============================================================================\
* Function   (名称): my_pipe();
* Description(功能):
* Called By  (被调): 1.
* Calls list (调用): 1.pipe();
* Input      (输入): 1.
* Output     (输出): 1.
* Return     (返回):
*         success  :
*         error    :
* Others     (其他): 1.
\*==============================================================================*/
void my_pipe(int *fds)
{
     if (pipe(fds) < 0)
     {
         err_sys("pipe error");
     }
}
