#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h> 
#include <netinet/if_ether.h> 
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <error.h>
#include <errno.h>

struct arpreq arpreq; 
int main(int argc, char* argv[])
{

        if(argc < 3)
        {
                printf("Usage: %s <IP> <name>\n",argv[0]);
                return 0;
        }
  int sd;
  struct arpreq arpreq;
  struct sockaddr_in *sin;
  struct in_addr ina;
  unsigned char *hw_addr;
// char *name = NULL;
  
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0)
  {
      perror("socket() error\n");
      exit(1);
  }
  printf("IP : %s\n", argv[1]);
  memset(&arpreq, 0, sizeof(struct arpreq));
  sin = (struct sockaddr_in *) &arpreq.arp_pa;
  memset(sin, 0, sizeof(struct sockaddr_in));
  sin->sin_family = AF_INET;
  ina.s_addr = inet_addr(argv[1]);
  memcpy(&sin->sin_addr, (char *)&ina, sizeof(struct in_addr));
  
  strcpy(arpreq.arp_dev, argv[2]); //wifi热点所用的网卡为wl0.1
  if (ioctl(sd, SIOCGARP, &arpreq) < 0)
  {
  	printf("ioctl error: %s", strerror(errno));
  }
     
  printf("\nentry has been successfully retreived\n");
  hw_addr = (unsigned char *) arpreq.arp_ha.sa_data;
  printf("MAC: %x:%x:%x:%x:%x:%x\n",  
  hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]);
  return 0;
} 
