/*
 * =====================================================================================
 *
 *       Filename:  my_fcntl.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 11時38分15秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
//#include <unistd.h>		//本文件中的函数用到的核心库函数都来自这些头文件
//#include <fcntl.h>

#include "my.h"

int my_fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ( (n = fcntl(fd, cmd, arg)) == -1)
		err_sys("fcntl error");
	return(n);
}

