/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : my_stdio.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.my_fgets();
*                2.my_fputs();
*                3.my_fclose()
*				 4.my_fopen();
*				 5.my_fdopen();
* History      :
\*==============================================================================*/

#include "my.h"

char* my_fgets(char *ptr, int n, FILE *stream)
{
    char *rptr;
    if ((rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
    {
        err_sys("fgets error");
    }

    return (rptr);
}

void my_fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF)
    {
        err_sys("fputs error");
    }
}

void my_fclose(FILE *fp)
{
    if (fclose(fp) != 0)
    {
        err_sys("fclose error");
    }
}

FILE * my_fdopen(int fd, const char *type)
{
    FILE    *fp;

    if ((fp = fdopen(fd, type)) == NULL)
    {
        err_sys("fdopen error");
    }

    return (fp);
}

FILE *my_fopen(const char *filename, const char *mode)
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
    {
        err_sys("fopen error");
    }

    return (fp);
}
