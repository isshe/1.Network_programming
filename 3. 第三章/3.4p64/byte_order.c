/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : byte_order.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/11
* Description  : 确定本机的字节序是大端还是小端.
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "../../include/my.h"

int main(int argc, char **argv)
{
    union
    {
        short   s;
        char    c[sizeof(short)];
    }un;

    un.s = 0x0102;

    printf("ubuntu: ");

    if (sizeof(short) == 2)
    {
        if (un.c[0] == 1 && un.c[1] == 2)
        {
            printf ("big-endian!\n");
        }
        else if (un.c[0] == 2 && un.c[1] == 1)
        {
            printf("little-endian!\n");
        }
        else
        {
            printf("unknow!\n");
        }
    }
    else
    {
        printf("sizeof(short) = %lu\n", sizeof(short));
    }

    return 0;
}
