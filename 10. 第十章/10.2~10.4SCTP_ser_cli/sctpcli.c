/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sctpcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/18
* Description  : SCTP流分回射客户程序
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

static void sctpstr_cli(FILE *fp, int sock_fd,
                        struct sockaddr *to, socklen_t tolen)
{
    struct  sockaddr_in     peeraddr;
    struct  sctp_sndrcvinfo  sri;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];
    int     out_sz = 0;
    int     rd_sz = 0;
    int     msg_flags = 0;
    socklen_t   len;

    bzero(&sri, sizeof(sri));

    while( fgets(sendline, MAXLINE, fp) != NULL )
    {
         if (sendline[0] != '[')
         {
             printf("Error: line must be of the form '[streamnum]text'\n");
             continue;
         }

         sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
         out_sz = strlen(sendline);         //发出去的大小吗?out_sz
         my_sctp_sendmsg(sock_fd, sendline, out_sz, to, tolen,
                         0, 0, sri.sinfo_stream, 0, 0);

         len = sizeof(peeraddr);
         rd_sz = my_sctp_recvmsg(sock_fd, recvline,
                                 sizeof(recvline),
                                 (SA *) &peeraddr,
                                 &len, &sri,
                                 &msg_flags);

         printf("From str: %d seq: %d (assoc: 0x%x):",
                 sri.sinfo_stream,
                 sri.sinfo_ssn,
                 (u_int)sri.sinfo_assoc_id);

         printf("%.*s", rd_sz, recvline);
    }
}


void
sctpstr_cli_echoall(FILE *fp, int sock_fd,
                    struct sockaddr *to,
                    socklen_t tolen)
{
     struct     sockaddr_in     peeraddr;
     struct     sctp_sndrcvinfo     sri;
     char   sendline[SCTP_MAXLINE];
     char   recvline[SCTP_MAXLINE];
     int    strsz = 0;
     int    rd_sz = 0;
     int    msg_flags = 0;
     int    i = 0;
     socklen_t  len;

     bzero(&sri, sizeof(sri));
     bzero(sendline, sizeof(sendline));

     while( fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL )
     {
          strsz =strlen(sendline);
          if (sendline[strsz - 1] == '\n')
          {
              sendline[strsz - 1] = '\0';
              strsz--;
          }

          for (i = 0; i < SERV_MAX_SCTP_STRM; i++)
          {
               snprintf(sendline + strsz, sizeof(sendline) - strsz,
                       ".msg.%d", i);
               my_sctp_sendmsg(sock_fd, sendline, sizeof(sendline),
                               to, tolen, 0, 0, i, 0, 0);
          }

          for (i = 0; i < SERV_MAX_SCTP_STRM; i++)
          {

               len = sizeof(peeraddr);
               rd_sz = my_sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
                                      (SA *) &peeraddr, &len,
                                      &sri, &msg_flags);

               printf("From str:%d seq:%d (assoc:0x%x):",
                      sri.sinfo_stream,
                      sri.sinfo_ssn,
                      (u_int)sri.sinfo_assoc_id);
               printf("%.*s\n", rd_sz, recvline);
          }
    }
}

int
main(int argc, char ** argv)
{
    int     sock_fd = 0;
    int     echo_to_all = 0;
    struct  sockaddr_in     servaddr;
    struct  sctp_event_subscribe    evnts;

    if (argc < 2)
    {
         err_quit("Missing host argument - use '%s host [echo]'\n", argv[0]);
    }
    else if (argc > 2)
    {
        printf("Echoing messages to all streams\n");
        echo_to_all = 1;
    }

    sock_fd = my_socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    bzero(&evnts, sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    my_setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS,
                    &evnts, sizeof(evnts));

    if ( 0 == echo_to_all )
    {
         sctpstr_cli(stdin, sock_fd, (SA *)&servaddr, sizeof(servaddr));
    }
    else
    {
        sctpstr_cli_echoall(stdin,
                            sock_fd,
                            (SA *)&servaddr,
                            sizeof(servaddr));
    }

    my_close(sock_fd);

    return (0);
}
