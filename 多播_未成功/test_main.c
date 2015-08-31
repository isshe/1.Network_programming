#include "my.h"
#include "test_main.h"

int udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

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
	
	sendfd = Udp_client(argv[1], argv[2], &sasend, &salen);
	recvfd = my_socket(sasend->sa_family, SOCK_DGRAM, 0);
	
	my_setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	
	sarecv = my_malloc(salen);
	memcpy(sarecv, sasend, salen);
	
	my_bind(recvfd, sarecv, salen);
	
	Mcast_join(recvfd, sasend, salen, NULL, 0);
	//Mcast_set_loop(sendfd, salen);
	
	if (my_fork() == 0)
	{
		recv_all(recvfd, salen);
	}
	
	send_all(sendfd, sasend, salen);
	
	return 0;
}




