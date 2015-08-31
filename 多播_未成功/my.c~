#include "my.h"
#include <sys/utsname.h>

#define SENDRATE 5		//发送频率，5秒一次

void recv_all(int, socklen_t);
void send_all(int, SA *, socklen_t);	//SA要注意

int main(int argc, char **argv)
{
	int sendfd = 0;
	int recvfd = 0;
	const int on = 1;
	socklen_t salen ;
	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	
	if (argc != 3)
	{
		err_quit("usage: sendrecv <IP-multicase-address><port#>");
	}
	
	sendfd = Udp_client(argv[1], argv[2], (void **)&sasend, &salen);
	recvfd = Socket(sasend->sa_family, SOCK_DGRAM, 0);
	
	setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(0n));
	
	sarecv = Malloc(salen);
	memcpy(sarecv, sasend, salen, NULL, 0);
	
	Bind(recvfd, sarecv, salen);
	
	Mcast_join(recvfd, sasend, salen, NULL, 0);
	//Mcast_set_loop(sendfd, salen);
	
	if (Fork() == 0)
	{
		recv_all(recvfd, salen);
	}
	
	send_all(sendfd, sasend, salen);
}

void send_all(int sendfd, SA 8sadest, socklen_t salen)
{
	char line[MAXLINE];
	struct utsname myname;
	
	if (uname(&myname) < 0)
	{
		err_sys("uname error");
	}
	
	snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());
	
	while(1)
	{
		Sendto(sendfd, line, strlen(line), 0, sadest, salen);
		sleep(SENDRATE);
	}
}

void recv_all(int recvfd, socklen_t salen)
{
	int n = 0;
	char line[MAXLINE + 1];
	socklen_t len;
	struct sockaddr *safrom;
	
	safrom = Malloc(salen);
	
	while(1)
	{
		len = salen;
		n = Recvfrom(recvfd, line, MAXLINE, 0, safrom, &len);
		line[n] = 0;
		printf("from %s:%s", Sock_ntop(safrom, len), line);
	}
}

