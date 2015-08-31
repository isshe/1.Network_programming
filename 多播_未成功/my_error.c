/*
 * =====================================================================================
 *
 *       Filename:  my_error.c
 *
 *    Description:  一些错误处理函数，参考《unix网络编程》
 *
 *        Version:  1.0
 *        Created:  2015年08月01日 星期六 11時43分12秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "my.h"

/* 打印错误信息 */
static void err_doit(int, int, const char *, va_list);

int daemon_proc;

/* 1.系统调用,致命错误,终止并打印错误信息 */
void err_sys(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);

	exit(1);
}

/* 2.系统调用,致命错误,终止并打印段错误等信息 */
void err_dump(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	
	abort();	//dump core and terminate
	
	exit(1);
}

/* 3.系统调用,非致命错误, 打印错误信息,返回 */
void err_ret(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_INFO, fmt, ap);
	va_end(ap);

	return ;
}

/* 4.不是致命错误,不是系统调用,打印错误信息并返回 */
void err_msg(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, LOG_INFO, fmt, ap);
	va_end(ap);

	return;
}

/* 5.致命错误,不是系统调用,打印错误信息并终止 */
void err_quit(const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);

	exit(1);
}

/* 打印错误信息 */
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	int 	errno_save = 0;
	int 	n = 0;
	char	buf[MAXLINE + 1];

	errno_save = errno;	//错误代码

#ifdef HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);	//safe
#else
	vsprintf(buf, fmt, ap);				//not safe
#endif

	n = strlen(buf);
	if (errnoflag)
	{
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	}
	
	strcat(buf, "\n");		//把参数2的字符串添加到参数1的后面,然后补'\0'
	
	if (daemon_proc)	//这一块不懂!!!
	{
		syslog(level, buf);	//写入系统日志! 这里有警告!!!
	}
	else
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}

}


