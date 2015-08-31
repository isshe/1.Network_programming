/* 函数声明上面的一行标号注释,是为了预留位置写说明.否则去掉应该会更好看*/

/*
 * =====================================================================================
 *
 *       Filename:  my.h
 *
 *    Description:  一些错误处理函数，参考《unix网络编程》
 *
 *        Version:  1.0
 *        Created:  2015年08月01日 星期六 11時43分12秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  chudai 
 *   Organization:  
 *
 * =====================================================================================
 */
 
#ifndef MY_ERROR_H_
#define MY_ERROR_H_

#include <stdlib.h>
#include <stdarg.h> 	//ANSI C header file.包含va_list等宏定义.
#include <syslog.h> 	//for syslog()
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>	
#include <arpa/inet.h>
#include <unistd.h>		//my_process.c, my_readwrite.c, 
#include <sys/stat.h>	//my_readwrite.c
#include <fcntl.h>		//my_readwrite.c
#include <sys/ioctl.h>	//my_ioctl.c
#include <sys/time.h>	//my_time.c
#include <sys/wait.h>	//my_process.c
#include <sys/utsname.h>//utsname结构!

#define MAXLINE 4096
#define SA struct sockaddr

/*一下五个在my_error.c中定义!*/
/* 1.系统调用,致命错误,终止并打印错误信息 */
void 	err_sys(const char *fmt, ...);

/* 2.系统调用,致命错误,终止并打印段错误等信息 */
void 	err_dump(const char *fmt, ...);

/* 3.系统调用,非致命错误, 打印错误信息,返回 */
void 	err_ret(const char *fmt, ...);

/* 4.不是致命错误,不是系统调用,打印错误信息并返回 */
void 	err_msg(const char *fmt, ...);

/* 5.致命错误,不是系统调用,打印错误信息并终止 */
void 	err_quit(const char *fmt, ...);


/*以下 在my_socket.c中定义*/
/* 1. */
int 	my_socket(int family, int type, int protocol);

/* 2. */
int		my_accept(int fd, struct sockaddr *sa, socklen_t *salenptr);

/* 3. */
void 	my_bind(int fd, const struct sockaddr *sa, socklen_t salen);

/* 4. */
void 	my_connect(int fd, const struct sockaddr *sa, socklen_t salen);

/* 5. */
void 	my_listen(int fd, int backlog);


/* 6. */
#ifdef	HAVE_POLL
int 	my_poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
#endif

/* 7. */
int 	my_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       				struct timeval *timeout);

/* 8. */
void 	my_getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);

/* 9. */
void 	my_getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr);

/* 10. */
void 	my_getsockopt(int fd, int level, int optname, 
						void *optval, socklen_t *optlenptr);

/* 11. */
void 	my_setsockopt(int fd, int level, int optname, 
						const void *optval, socklen_t optlen);
						
/* 12. */


/* my_malloc.c */
/* 1. */
void*	my_malloc(size_t size);

/* 2. */
void*	my_calloc(size_t n, size_t size);


/* my_process.c */
/* 1. */
pid_t	my_fork(void);

/* 2. */
pid_t 	my_wait(int *iptr);

/* 3. */
pid_t 	my_waitpid(pid_t pid, int *iptr, int options);


/* my_ioctl.c */
/* 1. */
int		my_ioctl(int fd, int request, void *arg);



/* my_time.c */
/* 1. */
void 	my_gettimeofday(struct timeval *tv, void *foo);



/* my_readwrite.c */
/* 1. */
int 	my_open(const char *pathname, int oflag, mode_t mode);

/* 2. */
void 	my_close(int fd);

/* 3. */
ssize_t my_read(int fd, void *ptr, size_t nbytes);

/* 4. */
void 	my_write(int fd, void *ptr, size_t nbytes);


/* my_fcntl.c */
/* 1. */
int 	my_fcntl(int fd, int cmd, int arg);


/* my_sendrecv.c */
/* 1. */
ssize_t my_recv(int fd, void *ptr, size_t nbytes, int flags);

/* 2. */
ssize_t my_recvfrom(int fd, void *ptr, size_t nbytes, int flags,
					struct sockaddr *sa, socklen_t *salenptr);

/* 3. */
ssize_t my_recvmsg(int fd, struct msghdr *msg, int flags);

/* 4. */
void 	my_send(int fd, const void *ptr, size_t nbytes, int flags);

/* 5. */
void 	my_sendto(int fd, const void *ptr, size_t nbytes, int flags,
					const struct sockaddr *sa, socklen_t salen);

/* 6. */
void 	my_sendmsg(int fd, const struct msghdr *msg, int flags);


#endif
