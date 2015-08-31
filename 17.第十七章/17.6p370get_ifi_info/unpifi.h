/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : unpifi.h
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/23
* Description  : unpifi.h头文件, 定义结构ifi_info{}
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#ifndef     _unp_ifi_h
#define     _unp_ifi_h

#include "my.h"
#include <net/if.h>

#define     IFI_NAME    16      //same as IFNAMSIZ in <net/if.h>
#define     IFI_HADDR   8       //allow for 64-bit EUI-64 in future

struct  ifi_info
{
     char       ifi_name[IFI_NAME];     //interface name, null-terminated
     short      ifi_index;              //interface index
     short      ifi_mtu;                //interface MTU
     u_char     ifi_haddr[IFI_HADDR];   //hardware address
     u_short    ifi_hlen;               //bytes in hardware address:0, 6, 8
    short       ifi_flags;              //IFF_xxx constants from <net/if.h>
    short       ifi_myflags;            //our own IFI_xxx flags
    struct  sockaddr    *ifi_addr;      //primary addr
    struct  sockaddr    *ifi_brdaddr;   //broadcast addr
    struct  sockaddr    *ifi_dstaddr;   //destination addr
    struct  ifi_info    *ifi_next;      //next of these structures
};

#define     IFI_ALIAS   1               //ifi_addr is an alias(别名)

struct  ifi_info    *get_ifi_info(int, int);
struct  ifi_info    *Get_ifi_info(int, int);
void    free_ifi_info(struct ifi_info *);



#endif
