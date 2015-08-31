/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : prifinfo.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/23
* Description  : 调用get_ifi_info()的prifinfo函数, 前者是网络编程作者写的
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"
#include "unpifi.h"

int main(int argc, char **argv)
{
     struct ifi_info    *ifi = NULL;
     struct ifi_info    *ifihead = NULL;
     struct sockaddr    *sa = NULL;
     u_char *ptr = NULL;
     int    i = 0;
     int    family = 0;
     int    doaliases = 0;

     if (argc < 3)
     {
          err_quit("usage: ./a.out <inet4|inet6> <doaliases>");
     }

     if (strcmp(argv[1], "inet4") == 0)
     {
          printf("family = AF_INET = %d\n", AF_INET);
          family = AF_INET;
     }
     else if (strcmp(argv[1], "inet6") == 0)
     {
     	  printf("family = AF_INET6 = %d\n", AF_INET6);
          family = AF_INET6;
     }
     else
     {
          err_quit("invalid <addr-family>\n");
     }

	 doaliases = atoi(argv[2]);
// 	doaliases = atoi("eth0");
// 	family = AF_INET;
     printf("doaliases = %d\n", doaliases);
    int count = 0;


     for (ifihead = ifi = Get_ifi_info(family, doaliases);
             ifi != NULL; ifi = ifi->ifi_next)
     {
//         DEBUG;
         count++;       //用完记得删除,联通count定义
          printf("\n%d: %s: ", count, ifi->ifi_name);

          if (ifi->ifi_index != 0)
          {
               printf("(%d)", ifi->ifi_index);
          }

          printf("<");
          if (ifi->ifi_flags & IFF_UP)          printf("UP ");
          if (ifi->ifi_flags & IFF_BROADCAST)   printf("BCAST ");
          if (ifi->ifi_flags & IFF_MULTICAST)   printf("MCAST ");
          if (ifi->ifi_flags & IFF_LOOPBACK)    printf("LOOP ");
          if (ifi->ifi_flags & IFF_POINTOPOINT) printf("P2P");
          printf(">\n");


/*         //这个是unix网络编程书上的//
          if ( (i= ifi->ifi_hlen) > 0 )
          {
               ptr = ifi->ifi_haddr;

               do
               {
                    printf("%s%x", (i == ifi->ifi_hlen) ? " ": ":",
                                    *ptr++);
               }while (--i > 0);
               printf("\n");
          }
*/
          printf("    hwaddr:");
          if (ifi->ifi_hlen > 0)
          {
              ptr = ifi->ifi_haddr;
            for (i = 0; i <6; i++)
            {
                  printf("%x", *(ptr + i));
                  if (i < 5)
                  {
                       printf(":");
                  }
                  else
                  {
                      printf("\n");
                  }
            }
        }//for


          if (ifi->ifi_mtu != 0)
          {
               printf("    MTU:%d\n", ifi->ifi_mtu);
          }

          if ( (sa = ifi->ifi_addr) != NULL )
          {
               printf("    IP addr: %s\n",
                       my_sock_ntop(sa, sizeof(*sa)));
          }

          if ( (sa = ifi->ifi_brdaddr) != NULL )
          {
               printf("    broadcast addr: %s\n",
                       my_sock_ntop(sa, sizeof(*sa)));
          }

          if ( (sa = ifi->ifi_dstaddr) != NULL )
          {
               printf("    destination addr: %s\n",
                       my_sock_ntop(sa, sizeof(*sa)));
          }

     }

     free_ifi_info(ifihead);

     exit(0);
}

