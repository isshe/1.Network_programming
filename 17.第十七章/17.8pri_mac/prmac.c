/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : prmac.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/24
* Description  : 输出一个主机的硬件地址
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
#include <net/if_arp.h>
#include "unpifi.h"

int main(int argc, char **argv)
{
     int    sockfd = 0;
     struct ifi_info    *ifi;
     struct arpreq      arpreq;
     struct sockaddr_in *sin;   //SA_IN
//     struct sockaddr 	*sin;   			//SA
     unsigned   char    *ptr;

     sockfd = my_socket(AF_INET, SOCK_DGRAM, 0);

     for (ifi = Get_ifi_info(AF_INET, 0);
          ifi != NULL;
          ifi = ifi->ifi_next)
     {
         DEBUG;
         printf("\n%s: ", my_sock_ntop(ifi->ifi_addr, sizeof(SA_IN)));


         //memcpy()这样有什么用? 答:把地址复制给arpreq.arp_pa, 通过sin;
         sin = (SA_IN *)&arpreq.arp_pa;         
         memcpy(sin, ifi->ifi_addr, sizeof(SA_IN));
         
//         sin->sin_family = AF_INET;
//		 memcpy(&arpreq.arp_pa, ifi->ifi_addr, sizeof(SA_IN));  
		 printf("%s: ", my_sock_ntop(&arpreq.arp_pa, sizeof(SA_IN)));       
         DEBUG;
         memcpy(arpreq.arp_dev, ifi->ifi_name, strlen(ifi->ifi_name)+1);
         printf("arp_dev = %s\n", arpreq.arp_dev);

         if (ioctl(sockfd, SIOCGARP, &arpreq) < 0)
         {
              DEBUG;
              err_ret("ioctl SIOCGARP");
              continue;     //注意这个continue
         }

         ptr = (unsigned char *)&arpreq.arp_ha.sa_data[0];
         printf("hwaddr = %x:%x:%x:%x:%x:%x\n\n",
                 *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
     }

     exit(0);
}
