//#include <sys/types.h>	//本文件中函数的核心库函数都来自这些头文件
//#include <sys/socket.h>
#include "my.h"
#include <netinet/sctp.h>


ssize_t
my_recv(int fd, void *ptr, size_t nbytes, int flags)
{
	ssize_t		n;

	if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
		err_sys("recv error");
	return(n);
}

ssize_t
my_recvfrom(int fd, void *ptr, size_t nbytes, int flags,
		 struct sockaddr *sa, socklen_t *salenptr)
{
	ssize_t		n;

	if ( (n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
		err_sys("recvfrom error");
	return(n);
}

ssize_t
my_recvmsg(int fd, struct msghdr *msg, int flags)
{
	ssize_t		n;

	if ( (n = recvmsg(fd, msg, flags)) < 0)
		err_sys("recvmsg error");
	return(n);
}

int 	 
my_sctp_recvmsg(int s, void *msg, size_t len, 
				struct sockaddr *from, socklen_t *fromlen,
				struct sctp_sndrcvinfo *sinfo, int *msg_flags)
{
	int ret = 0;
	
	ret = sctp_recvmsg(s, msg, len, from, fromlen, sinfo, msg_flags);
	if (ret < 0)
	{
		err_sys("sctp_recvmsg error");
	}
	
	return (ret);
}




void
my_send(int fd, const void *ptr, size_t nbytes, int flags)
{
	if (send(fd, ptr, nbytes, flags) != (ssize_t)nbytes)
		err_sys("send error");
}

void
my_sendto(int fd, const void *ptr, size_t nbytes, int flags,
	   const struct sockaddr *sa, socklen_t salen)
{
	if (sendto(fd, ptr, nbytes, flags, sa, salen) != (ssize_t)nbytes)
		err_sys("sendto error");
}

void
my_sendmsg(int fd, const struct msghdr *msg, int flags)
{
	unsigned int	i;
	ssize_t			nbytes;

	nbytes = 0;	/* must first figure out what return value should be */
	for (i = 0; i < msg->msg_iovlen; i++)
		nbytes += msg->msg_iov[i].iov_len;

	if (sendmsg(fd, msg, flags) != nbytes)
		err_sys("sendmsg error");
}

int 
my_sctp_sendmsg(int s, void *data, size_t len, 
				struct sockaddr *to, socklen_t tolen,
				uint32_t ppid, uint32_t flags,
				uint16_t stream_no, uint32_t timetolive,
				uint32_t context)
{
	int ret;
	
	ret = sctp_sendmsg(s, data, len, to, tolen, ppid, 
						flags, stream_no, timetolive, context);
						
	if (ret < 0)
	{
		err_sys("sctp_sendmsg error");
	}
	
	return (ret);
}

