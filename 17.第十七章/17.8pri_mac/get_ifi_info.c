/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : get_ifi_info.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/23
* Description  : 发出SIOCGIFCONF请求, 以获取接口配置
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "unpifi.h"
//#include <net/if_dl.h>

struct ifi_info *get_ifi_info(int family, int doaliases)
{

/* 以下是第一部分: 发出SIOCGIFCONF请求以获取接口配置 */
    struct     ifi_info    *ifi = NULL;            //自定义的结构!
     struct     ifi_info    *ifihead = NULL;
     struct     ifi_info    **ifipnext = NULL;      //何用?
     int        sockfd = 0;
     int        len = 0;
     int        lastlen = 0;            //何用?
     int        flags = 0;              //何用?
     int        myflags = 0;            //
     int        idx = 0;
     int        hlen = 0;               //?
     char       *ptr = NULL;            //指向下一个结构
     char       *buf = NULL;
     char       lastname[IFNAMSIZ] = {0};
     char       *cptr = NULL;
     char       *haddr = NULL;
     char       *sdlname = NULL;
     struct     ifconf      ifc;            //系统结构
     struct     ifreq       *ifr = NULL;    //系统结构
     struct     ifreq       ifrcopy;
     SA_IN 		*sinptr = NULL;
     SA_IN6    	*sin6ptr = NULL;

	 sockfd = my_socket(AF_INET, SOCK_DGRAM, 0); 		//居然漏了这句!!!

     lastlen = 0;       //?

     len = 100 * sizeof(struct ifreq);       //initial buffer size guess

     while (1)
     {
          buf = my_malloc(len);
          ifc.ifc_len = len;            //此处可对照p368的图
          ifc.ifc_buf = buf;

          //ioctl成功以后会修改ifc_len和ifc_buf!
          if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
          {
               if (errno != EINVAL || lastlen != 0)
               {
               		DEBUG; 		//!!!
                    err_sys("ioctl error");
               }
          }
          else
          {
               //第一次不是铁定失败(退不去循环)?lastlen = 0 ?! 是的!!!
               if (ifc.ifc_len == lastlen)
               {
                   break;       //success, len has not changed.
               }
               lastlen = ifc.ifc_len;
          }

          len += 10 * sizeof(struct ifreq);     //increment(增值)
          free(buf);    //超帅!!!
     }

     ifihead = NULL;        //??
     ifipnext = &ifihead;   //?? 构造一个链表?
     lastname[0] = 0;       //??
     sdlname = NULL;        //??

     DEBUG;
    printf("lastlen = %d\n", lastlen);

/* 以上是第一部分: 发出SIOCGIFCONF请求以获取接口配置 */

/* 以下是第二部分: 处理接口配置 */
     for (ptr = buf; ptr < buf + ifc.ifc_len; )         //注意这里
     {
          ifr = (struct ifreq *) ptr;

            //设置len???
#ifdef     HAVE_SOCKADDR_SA_LEN
           len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
           switch (ifr->ifr_addr.sa_family)
           {
#ifdef     IPV6
               case AF_INET6:
                   len = sizeof(SA_IN6);
                   break;
#endif
               case AF_INET:
               default:
                   len = sizeof(struct sockaddr);
                   break;
           }
#endif  //HAVE_SOCKADDR_SA_LEN

/*           DEBUG;
           printf("ifi_name = %s\n", ifr->ifr_name);
           printf("sizeof(ifr->ifi_name) = %lu, len = %d\n",
                   sizeof(ifr->ifr_name), len);
*/

           //原本是:sizeof(ifr->ifr_name) + len;  //??? for next one in buffer
           ptr += sizeof(struct ifreq);
#ifdef     HAVE_SOCKADDR_DL_STRUCT
           //assume that AF_LINK precedes AF_INET or AF_INET6
           if (ifr->ifr_addr.sa_family == AF_LINK)
           {
                struct sockaddr_dl *sdl = NULL;
                sdl = (struct sockaddr_dl *)&ifr->ifr_addr;
                sdlname = ifr->ifr_name;        //??
                idx = sdl->sdl_index;           //??
                haddr = sdl->sdl_data + sdl->sdl_nlen;      //???
                hlen = sdl->sdl_alen;
           }
#endif
           if (ifr->ifr_addr.sa_family != family)
           {
                continue;       //ignore if not desired(请求的) address family
           }

           myflags = 0;

           //查找首次出现在字符串中的字符
           if ( (cptr = strchr(ifr->ifr_name, ':')) != NULL )
           {
                *cptr = 0;      //replace colon with null
           }

           if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0)
           {
                if (doaliases == 0)
                {
                    continue;       //already processed this interface
                }

                myflags = IFI_ALIAS;        //??
           }

           memcpy(lastname, ifr->ifr_name, IFNAMSIZ);


            //获取标志(flags);
           ifrcopy = *ifr;  //??为了ifr不被覆盖,所以用多一个,用于获取flags
           my_ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
           flags = ifrcopy.ifr_flags;       //获取了flags
           if ( (flags & IFF_UP) == 0 )
           {
               DEBUG;
               continue;       //ignore if interface not up!!
           }

/* 以上是第二部分: 处理接口配置 */

/* 第三部分: 分配并初始化ifi_info结构 */
           ifi = my_calloc(1, sizeof(struct ifi_info)); //??
           *ifipnext = ifi;         //prev points to this one goes here
/*漏了这句*/ ifipnext = &ifi->ifi_next;	/* pointer to next one goes here */

           //下面一大大大块设置ifi结构
           ifi->ifi_flags = flags; //IFF_XXX values
           ifi->ifi_myflags = myflags;

           //获取 MTU 最大传输单元
#if defined(SIOCGIFMTU) //&& defined(HAVE_STRUCT_IFREQ_IFR_MTU)  //??
           my_ioctl(sockfd, SIOCGIFMTU, &ifrcopy);
           ifi->ifi_mtu = ifrcopy.ifr_mtu;
#else
           ifi->ifi_mtu = 0;
#endif

           //ifi是自定义的结构, ifr是系统结构
           memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
           ifi->ifi_name[IFI_NAME - 1] = '\0';

           //if the sockaddr_dl is from a different interface, ignore it
           //以下这块还是设置ifi结构!!!
           if (sdlname == NULL || strcmp(sdlname, ifr->ifr_name) != 0)
           {
                idx = hlen = 0;         //???
           }
           ifi->ifi_index = idx;

           //如果没有sockaddr_dl结构,就用以下方法获取mac!
           if (hlen == 0)
           {
                my_ioctl(sockfd, SIOCGIFHWADDR, &ifrcopy);
                haddr = ifrcopy.ifr_hwaddr.sa_data;

                hlen = sizeof(ifrcopy.ifr_hwaddr);      //这里可能错
                ifi->ifi_hlen = hlen;
           }

           if (ifi->ifi_hlen > IFI_HADDR)
           {
                ifi->ifi_hlen = IFI_HADDR;
           }

           if (hlen)
           {
                memcpy(ifi->ifi_haddr, haddr, ifi->ifi_hlen);
           }

/*           DEBUG;       //
           int k = 0;
           for (k = 0; k < 6; k++)
           {
                printf("%x:", *(haddr+k));
           }
           printf("\n");
*/
/* 以上第三部分: 分配并初始哈ifi_info结构 */

/* 以下第四部分: 获取并返回接口地址 */
           switch (ifr->ifr_addr.sa_family)
           {
               case AF_INET:
                   sinptr = (SA_IN *) &ifr->ifr_addr;  //???
                   ifi->ifi_addr = my_calloc(1, sizeof(SA_IN));
                   memcpy(ifi->ifi_addr, sinptr, sizeof(SA_IN));

#ifdef         SIOCGIFBRDADDR //broadcast addr
                   if (flags & IFF_BROADCAST)
                   {
                        my_ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
                        sinptr = (SA_IN *) &ifrcopy.ifr_broadaddr;
                        ifi->ifi_brdaddr = my_calloc(1, sizeof(SA_IN));
                        memcpy(ifi->ifi_brdaddr, sinptr, sizeof(SA_IN));
                   }
#endif

#ifdef         SIOCGIFDSTADDR  //destination addr
                   if (flags & IFF_POINTOPOINT)
                   {
                        my_ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
                        sinptr = (SA_IN *) &ifrcopy.ifr_dstaddr;
                        ifi->ifi_dstaddr = my_calloc(1, sizeof(SA_IN));
                        memcpy(ifi->ifi_dstaddr, sinptr, sizeof(SA_IN));
                   }
#endif

                   break;       //!!!

               case AF_INET6:
                   sin6ptr = (SA_IN6 *) &ifr->ifr_addr;
                   ifi->ifi_addr = my_calloc(1, sizeof(SA_IN6));
                   memcpy(ifi->ifi_addr, sin6ptr, sizeof(SA_IN6));

#ifdef         SIOCGIFDSTADDR  //destination addr
                   if (flags & IFF_POINTOPOINT)
                   {
                        my_ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
                        sin6ptr = (SA_IN6 *) &ifrcopy.ifr_dstaddr;
                        ifi->ifi_dstaddr = my_calloc(1, sizeof(SA_IN6));
                        memcpy(ifi->ifi_dstaddr, sin6ptr, sizeof(SA_IN6));
                   }
#endif
                   break;

               default:
                   break;

           }
     }

     free(buf);
     return(ifihead);       //???
/* 以上第四部分: 获取并返回接口地址 */
}

void free_ifi_info(struct ifi_info *ifihead)
{
    struct ifi_info     *ifi = NULL;
    struct ifi_info     *ifinext = NULL;

    for (ifi = ifihead; ifi != NULL; ifi = ifinext)
    {
         if (ifi->ifi_addr != NULL)
         {
             free(ifi->ifi_addr);
         }

         if (ifi->ifi_brdaddr != NULL)
         {
              free(ifi->ifi_brdaddr);
         }

         if (ifi->ifi_dstaddr != NULL)
         {
             free(ifi->ifi_dstaddr);
         }

         ifinext = ifi->ifi_next;       //can't fetch ifi_next after free()
         free(ifi);         //帅!!!
    }
}

struct ifi_info *Get_ifi_info(int family, int doaliases)
{
     struct     ifi_info    *ifi;

     if ( (ifi = get_ifi_info(family, doaliases)) == NULL )
     {
         err_quit("get_ifi_info error");
     }

     return (ifi);
}
