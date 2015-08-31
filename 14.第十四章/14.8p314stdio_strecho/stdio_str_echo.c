/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : stdio_str_echo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/21
* Description  : 重新编写图5-3的TCP回射服务程序,改用标准I/O
* Function list: 1.str_echo();
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

void
str_echo(int sockfd)
{
     char   line[MAXLINE] = {0};
     FILE   *fpin;
     FILE   *fpout;

     fpin = my_fdopen(sockfd, "r");
     fpout= my_fdopen(sockfd, "w");

     while ( my_fgets(line, MAXLINE, fpin) != NULL )
     {
          my_fputs(line, fpout);
     }
}
