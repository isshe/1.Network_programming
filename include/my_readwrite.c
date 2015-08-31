/*
 * =====================================================================================
 *
 *       Filename:  my_readwrite.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 11時24分50秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
//include <unistd.h>		//此文件中的函数所用的核心库函数都在这些头文件中
//#include <sys/stat.h>	
//#include <fcntl.h>
	
#include "my.h"

int my_open(const char *pathname, int oflag, mode_t mode)
{
	int		fd;

	if ( (fd = open(pathname, oflag, mode)) == -1)
		err_sys("open error for %s", pathname);
	return(fd);
}


void my_close(int fd)
{
	if (close(fd) == -1)
		err_sys("close error");
}

ssize_t my_read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		err_sys("read error");
	return(n);
}

void my_write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}

