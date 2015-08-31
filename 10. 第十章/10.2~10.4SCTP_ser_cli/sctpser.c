/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sctpser.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/18
* Description  : sctp流分回射服务器程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

int
main(int argc, char **argv)
{
    int     sock_fd = 0;
    int     msg_flags = 0;
    char    readbuf[BUFFSIZE];
    struct  sockaddr_in     cliaddr;
    struct  sockaddr_in     servaddr;
    struct  sctp_sndrcvinfo sri;
    struct  sctp_event_subscribe evnts;
    int     stream_increment = 1;               //注意这里
    size_t  rd_sz;
    socklen_t   len;

    if ( 2 == argc )
    {
         stream_increment = atoi(argv[1]);
    }

    sock_fd = my_socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    my_bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));

    //
    bzero(&evnts, sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    my_setsockopt(sock_fd, IPPROTO_SCTP,
                    SCTP_EVENTS, &evnts,sizeof(evnts));

    my_listen(sock_fd, LISTENQ);

    while ( 1 )
    {
         len = sizeof(struct sockaddr_in);
         rd_sz = my_sctp_recvmsg(sock_fd, readbuf,
                                 sizeof(readbuf),
                                 (SA *) &cliaddr,
                                 &len, &sri, &msg_flags);

         if (stream_increment)
         {
//              sri.sinfo_stream++;

              //与最大流号比较
/*              if (sri.sinfo_stream >=
                  sctp_get_no_strms(sock_fd, (SA *) &cliaddr, len))
              {
                  //流号清0
                  sri.sinfo_stream = 0;
              }
*/
            if (sri.sinfo_stream > 9)
            {
                  sri.sinfo_stream = 0;
            }
         }
         my_sctp_sendmsg(sock_fd, readbuf, rd_sz,
                         (SA *) &cliaddr, len,
                         sri.sinfo_ppid, sri.sinfo_flags,
                         sri.sinfo_stream, 0, 0);

    }
}
