/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : test_read_write.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/15
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    char buf[1024] ={0};

    int n = 0;

    while(1)
    {
        if ((n = read(fileno(stdin), buf, 1024)) < 0)
        {
             printf("read error\n");
        }
        else
        {
            printf("read success!\n");
            if (n == 0)
            {
                printf("读入EOF, 退出!\n");
                 return 0;
            }
        }

        write(fileno(stdout), buf, n);
    }
    return 0;
}
