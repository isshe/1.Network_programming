/*
 * =====================================================================================
 *
 *       Filename:  my_process.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 10時56分57秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

//#include <unistd.h> //这个文件中的函数用到的库函数都来自这个头文件
#include "my.h"

pid_t my_fork(void)
{
	pid_t	pid;

	if ( (pid = fork()) == -1)
		err_sys("fork error");
	return(pid);
}


pid_t my_wait(int *iptr)
{
	pid_t	pid;

	if ( (pid = wait(iptr)) == -1)
		err_sys("wait error");
	return(pid);
}


pid_t my_waitpid(pid_t pid, int *iptr, int options)
{
	pid_t	retpid;

	if ( (retpid = waitpid(pid, iptr, options)) == -1)
		err_sys("waitpid error");
	return(retpid);
}

