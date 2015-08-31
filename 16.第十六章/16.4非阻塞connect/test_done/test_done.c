/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : test_done.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 测试一下类似汇编的标志是怎么用的
*                   结果: 顺序执行下去, 并没有理会标志.标志或许只是用来跳转
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{

    int n = 0;

    if ( 0 != n)
    {
         goto done;
         printf("n = %d\n", n);
    }

    printf("done 上面!\n");

done:
    printf("done 下面!\n");

    return 0;
}
