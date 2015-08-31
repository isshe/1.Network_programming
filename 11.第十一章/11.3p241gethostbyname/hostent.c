/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : hostent.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/19
* Description  : 调用gethostbyname()并显示返回信息
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


#include "my.h"

int
main(int argc, char **argv)
{
    char    *ptr = NULL;
    char    **pptr = NULL;
    char    str[INET_ADDRSTRLEN];
    struct  hostent *hptr;

    while ( --argc > 0 )
    {
         ptr = *++argv;
         if ( (hptr = gethostbyname(ptr)) == NULL )
         {
             err_msg("gethostbyname error for host: %s: %s",
                     ptr, hstrerror(h_errno));
             continue;
         }

         printf("official hostname: %s\n", hptr->h_name);

         for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
         {
              printf("\talias: %s\n", *pptr);
         }

         switch (hptr->h_addrtype)
         {
             case AF_INET:
                 pptr = hptr->h_addr_list;
                 for( ; *pptr != NULL; pptr++ )
                 {
                      printf("\taddress: %s\n",
                              my_inet_ntop(hptr->h_addrtype, *pptr,
                                            str, sizeof(str)));
                 }
                 break;

             default:
                 err_ret("unknow address type");
                 break;
         }
    }

    exit(0);
}
