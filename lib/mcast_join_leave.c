/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : mcast_join.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/26
* Description  : 加入一个多播组
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "mcast_join_leave.h"
//#include <net/if.h>


//第一部分:加入一个多播组:IP无关套接字//
int mcast_join(int sockfd, const SA *grp, socklen_t grplen,
               const char *ifname, u_int ifindex)
{
#ifdef  MCAST_JOIN_GROUP
    struct  group_req   req;

    if (ifindex > 0)
    {
         req.gr_interface = ifindex;
    }
    else if (ifname != NULL)
    {
         if ( (req.gr_interface = if_nametoindex(ifname)) == 0)
         {
              errno = ENXIO;        //i/f name not found
              DEBUG;
              printf("#errno = ENXIO!\n");      //+
              return(-1);
         }
    }
    else
    {
         req.gr_interface = 0;
    }

    if (grplen > sizeof(req.gr_group))
    {
         errno = EINVAL;
         DEBUG;
         printf("#errno = EINVAL!\n");
         return (-1);
    }

    memcpy(&req.gr_group, grp, grplen);
    return (setsockopt(sockfd, my_family_to_level(grp->sa_family),
                       MCAST_JOIN_GROUP, &req, sizeof(req)));
//第一部分:加入一个多播组:IP无关套接字//

#else

//第二部分:加入一个多播组:IPv4套接字//
    switch (grp->sa_family)
    {
         case AF_INET:
             struct     ip_mreq     mreq;
             struct     ifreq       ifreq;

             //填充多播地址
             memcpy(&mreq.imr_multiaddr,
                    &((const SA_IN *) grp)->sin_addr,
                    sizeof(struct in_addr));

             //通过接口索引或者接口名字获取接口地址!
             //接口地址是主要的.
             if (ifindex > 0)
             {
                  if (if_indextoname(ifindex, ;ifreq.ifr_name) == NULL)
                  {
                      errno = ENXIO;
                      DEBUG;
                      printf("errno = ENXIO!\n");
                      return(-1);
                  }

                  goto doioctl;
             }
             else if (ifname != NULL)
             {
                  strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
                  
doioctl:
                  if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
                  {
                       DEBUG;
                       printf("ioctl error!\n");    //+
                       return (-1);
                  }

                  memcpy(&mreq.imr_interface,
                         &((SA_IN *) &ifreq.ifr_addr)->sin_addr,
                         sizeof(struct in_addr));
             }
             else
             {
                  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
             }

             return (setsockopt(sockfd, IPPROTO_IP,
                                IP_ADDR_MEMBERSHIP,
                                &mreq, sizeof(mreq)));

//第二部分:加入一个多播组:IPv4套接字//

//第三部分:加入一个多播组:IPv6套接字//
#ifdef  IPV6
         case AF_INET6:
             struct ipv6_mreq   mreq6;

             memcpy(&mreq6, ipv6mr_multiaddr,
                    &((const SA_IN6 *) grp)->sin6_addr,
                    sizeof(struct in6_addr));

             if (ifindex > 0)
             {
                  mreq6.ipv6mr_interface = ifindex;
             }
             else if (ifname != NULL)
             {
                  if ((mreq6.ipv6mr_interface
                              = if_nametoindex(ifname)) == 0)
                  {
                      errno = ENXIO;
                      DEBUG;
                      printf("#errno = ENXIO!\n");  //+
                      return (-1);
                  }
             }
             else
             {
                  mreq6.ipv6mr_interface = 0;
             }

             return(setsockopt(sockfd, IPPROTO_IPV6,
                               IPV6_JOIN_GROUP,
                               &mreq6, sizeof(mreq60)));
#endif

         default:
             errno = EAFNOSUPPORT;
             DEBUG;
             printf("errno = EAFNOSUPPORT!\n");
             return (-1);
    }

#endif
}

//第三部分: 加入一个多播组:IPv6套接字//
