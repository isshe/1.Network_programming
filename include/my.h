/* 函数声明上面的一行标号注释,是为了预留位置写说明.否则去掉应该会更好看*/

/* =====================================================================================
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

#ifndef _MY_H_
#define _MY_H_

#include <stdlib.h>
#include <stdarg.h> 	//ANSI C header file.包含va_list等宏定义.
#include <syslog.h> 	//for syslog()
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//htons();
#include <unistd.h>		//my_process.c, my_readwrite.c,
#include <sys/stat.h>	//my_readwrite.c
#include <fcntl.h>		//my_readwrite.c
#include <sys/ioctl.h>	//my_ioctl.c
#include <sys/time.h>	//my_time.c
#include <sys/wait.h>	//my_process.c
#include <sys/utsname.h>//utsname结构!
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
#include <poll.h>       //poll()
#include <sys/stropts.h>
#include <netinet/sctp.h>    	//sctp, 编译的时候加 -lsctp
#include <netdb.h> 				//gethostbyname()
#include <sys/un.h> 			//unix相关,例如:sockaddr_un{};
#include <net/if.h>
#include <pthread.h>            //编译的时候记得加 -lpthread

#define MAXLINE 			4096
#define SCTP_MAXLINE 		800
#define BUFFSIZE 			8192
#define SA 					struct sockaddr
#define SA_IN 				struct sockaddr_in
#define SA_IN6 				struct sockaddr_in6
#define SA_DL 				struct sockaddr_dl
#define SERV_MAX_SCTP_STRM 	10

#define	LISTENQ				1024			// 2nd argument to listen()
#define	SERV_PORT			9877
#define	SERV_PORT_STR		"9877"			// TCP and UDP client-servers

#define max(a, b)           ((a) > (b) ? (a) : (b))
#define DEBUG 				(printf("#%s:%d: \n", __FILE__, __LINE__))

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

const char *hstrerror(int err);


/*以下 在my_socket.c中定义*/
int 	my_socket(int family, int type, int protocol);
int		my_accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void 	my_bind(int fd, const struct sockaddr *sa, socklen_t salen);
void 	my_connect(int fd, const struct sockaddr *sa, socklen_t salen);
void 	my_listen(int fd, int backlog);
void 	my_getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);
void 	my_getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr);
void 	my_getsockopt(int fd, int level, int optname,
						void *optval, socklen_t *optlenptr);
void 	my_setsockopt(int fd, int level, int optname,
						const void *optval, socklen_t optlen);
void 	my_shutdown(int fd, int how);

/* my_sock.c */
//这两个函数其实差不多, _host()这个,不要应该问题也不大, 抄书上的!!!
char*	my_sock_ntop(const struct sockaddr *sa, socklen_t salen);
char* 	my_sock_ntop_host(const struct sockaddr *sa, socklen_t salen);



/* my_malloc.c */
void*	my_malloc(size_t size);
void*	my_calloc(size_t n, size_t size);


/* my_process.c */
pid_t	my_fork(void);
pid_t 	my_wait(int *iptr);
pid_t 	my_waitpid(pid_t pid, int *iptr, int options);


/* my_ioctl.c */
/* 1. */
int		my_ioctl(int fd, int request, void *arg);



/* my_time.c */
void 	my_gettimeofday(struct timeval *tv, void *foo);



/* my_readwrite.c */
int 	my_open(const char *pathname, int oflag, mode_t mode);
void 	my_close(int fd);
ssize_t my_read(int fd, void *ptr, size_t nbytes);
void 	my_write(int fd, void *ptr, size_t nbytes);


/* my_fcntl.c */
int 	my_fcntl(int fd, int cmd, int arg);


/* my_sendrecv.c */
ssize_t my_recv(int fd, void *ptr, size_t nbytes, int flags);
ssize_t my_recvfrom(int fd, void *ptr, size_t nbytes, int flags,
					struct sockaddr *sa, socklen_t *salenptr);
ssize_t my_recvmsg(int fd, struct msghdr *msg, int flags);
int 	 my_sctp_recvmsg(int s, void *msg, size_t len,
						 struct sockaddr *from, socklen_t *fromlen,
						 struct sctp_sndrcvinfo *sinfo, int *msg_flags);

void 	my_send(int fd, const void *ptr, size_t nbytes, int flags);
void 	my_sendto(int fd, const void *ptr, size_t nbytes, int flags,
					const struct sockaddr *sa, socklen_t salen);
void 	my_sendmsg(int fd, const struct msghdr *msg, int flags);
int		 my_sctp_sendmsg(int s, void *data, size_t len,
							struct sockaddr *to, socklen_t tolen,
							uint32_t ppid, uint32_t flags,
							uint16_t stream_no, uint32_t timetolive,
							uint32_t context);

/* my_stdio.c */
char* my_fgets(char *ptr, int n, FILE *stream);
void  my_fputs(const char *ptr, FILE *stream);
void  my_fclose(FILE *fp);
FILE* my_fopen(const char *filename, const char *mode);
FILE* my_fdopen(int fd, const char *type);

/* my_readline.c */
ssize_t my_readline(int fd, void *ptr, size_t maxlen);

/* my_writen.c */
/*一次写n个字节 */
void my_writen(int fd, void *ptr, size_t nbytes);

/* my_inet.c */
void my_inet_pton(int family, const char *strptr, void *addrptr);
const char* my_inet_ntop(int family, const void *addrptr,
						 char *strptr, size_t len);

/* my_signal.c */
/*为了把
 *    void (*signal(int signo, void (*func)(int)))(int);
 *简化.所以用了一下typedef!
 */
typedef void Sigfunc(int);

Sigfunc * my_signal(int signo, Sigfunc *func);
void my_sigaddset(sigset_t *set, int signo);
void my_sigdelset(sigset_t *set, int signo);

/*sigemptyset()用来将参数set信号集初始化并清空。
 * 执行成功则返回0，如果有错误则返回-1。
 */
void my_sigemptyset(sigset_t *set);

/*sigfillset()用来将参数set信号集初始化，
 * 然后把所有的信号加入到此信号集里即将所有的信号标志位置为1，屏蔽所有的信号。
 * 它是一个宏实现.
 */
void my_sigfillset(sigset_t *set);

/*sigismember()用来测试参数signum 代表的信号是否已加入至参数set信号集里。
 *  如果信号集里已有该信号则返回1，否则返回0。如果有错误则返回-1。
 */
int my_sigismember(const sigset_t *set, int signo);
void my_sigpending(sigset_t *set);

/*一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。
   sigprocmask()可以用来检测或改变目前的信号屏蔽字，
   其操作依参数how来决定，如果参数oldset不是NULL指针，
   那么目前的信号屏蔽字会由此指针返回..
   如果set是一个非空指针，则参数how指示如何修改当前信号屏蔽字。
   每个进程都有一个用来描述哪些信号递送到进程时将被阻塞的信号集，
   该信号集中的所有信号在递送到进程后都将被阻塞。
参数how的取值不同，带来的操作行为也不同，该参数可选值如下：
    1．SIG_BLOCK:　该值代表的功能是将newset所指向的信号集中所包含的信号
    加到当前的信号掩码中，作为新的信号屏蔽字。
    2．SIG_UNBLOCK:将参数newset所指向的信号集中的信号从当前的信号掩码中移除。
    3．SIG_SETMASK:设置当前信号掩码为参数newset所指向的信号集中所包含的信号。
注意事项：sigprocmask()函数只为单线程的进程定义的，
        在多线程中要使用pthread_sigmask变量，在使用之前需要声明和初始化..
    执行成功返回0，失败返回-1。
*/
void my_sigprocmask(int how, const sigset_t *set, sigset_t *oset);


/* my_select.c */

//#ifdef	HAVE_POLL
int 	my_poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
//#endif

int 	my_select(int nfds, fd_set *readfds,
					fd_set *writefds, fd_set *exceptfds,
					struct timeval *timeout);

/* sctp_getnostrms.c */ //  在<<unix网络编程(1)>>p224页用到.
//获取最大流号//
int 	sctp_get_no_strms(int sock_fd,
						  struct sockaddr *to,
						  socklen_t tolen);

/* my_tcp.c */
//使用getaddrinfo()
//用于简化访问getaddrinfo()的TCP接口函数
int tcp_connect(const char *host, const char *serv);
int	tcp_listen(const char *hostname,
				  const char *service,
				  socklen_t *addrlenp);

/* my_udp.c */
//使用getaddrinfo()
//用于简化访问getaddrinfo的UDP接口函数
int udp_client(const char *host, const char *serv,
				SA **saptr, socklen_t *lenp);
int udp_connect(const char *hostname, const char *service);
int udp_server(const char *hostname,
				const char *service,
				socklen_t *lenptr);


/* my_pipe.c */
void my_pipe(int *fds);


/* mcast_XXX.c 系列 */
/* mcast_join_leave.c */
int mcast_join(int sockfd, const SA *grp, socklen_t grplen,
               const char *ifname, u_int ifindex);

/* family_to_level.c */
//被mcast_join()调用//
int my_family_to_level(int family);

/* sockfd_to_family.c */
//被mcast_set_loop()调用//
int my_sockfd_to_family(int sockfd);


/* my_pthread.c */
void my_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                       void * (*func)(void *), void *arg);

void my_pthread_join(pthread_t tid, void **status);
void my_pthread_detach(pthread_t tid);
void my_pthread_kill(pthread_t tid, int signo);
void my_pthread_mutexattr_init(pthread_mutexattr_t *attr);
void my_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
                                     int flag);
void my_pthread_mutex_init(pthread_mutex_t *mptr,
                           pthread_mutexattr_t *attr);
void my_pthread_mutex_lock(pthread_mutex_t *mptr);
void my_pthread_mutex_unlock(pthread_mutex_t *mptr);
void my_pthread_cond_broadcast(pthread_cond_t *cptr);
void my_pthread_cond_signal(pthread_cond_t *cptr);
void my_pthread_cond_wait(pthread_cond_t *cptr,
                          pthread_mutex_t *mptr);
void my_pthread_cond_timewait(pthread_cond_t *cptr,
                              pthread_mutex_t *mptr,
                              const struct timespec *tsptr);
void my_pthread_once(pthread_once_t *ptr, void (*func)(void));
void my_pthread_key_create(pthread_key_t *key,
                           void (*func)(void *));
void my_pthread_setspecific(pthread_key_t key, const void *value);

#endif
