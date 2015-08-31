/*
 * =====================================================================================
 *
 *       Filename:  my_malloc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 10時48分52秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "my.h"
//#include <stdlib.h> 	//此文件中的库函数都是属于这个头文件

void* my_calloc(size_t n, size_t size)
{
	void *ptr = NULL;
	if ((ptr = calloc(n, size)) == NULL)
	{
		err_sys("calloc error");
	}

	return (ptr);
}

void* my_malloc(size_t size)
{
	void *ptr;
	if ((ptr = malloc(size)) == NULL)
	{
		err_sys("malloc error");
	}

	return (ptr);
}
