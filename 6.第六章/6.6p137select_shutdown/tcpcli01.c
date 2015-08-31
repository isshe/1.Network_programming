/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : tcpcli01.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/13
* Description  :
* Function list: 1.main()
*                2.str_cli()
*                3.
* History      :
\*==============================================================================*/

#include "my.h"
//#include "tcpcli01.h"

//5.5节
static void str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];

    while (my_fgets(sendline, MAXLINE, fp) != NULL)
    {
        my_writen(sockfd, sendline, strlen(sendline));

        if (my_readline(sockfd, recvline, MAXLINE) == 0)
        {
            err_quit("str_cli: server terminated prematurely");
        }

        my_fputs(recvline, stdout);
    }
}

static max(int x, int y)
{
    return (x>y? : x,y);
}



//6.5节图6-9
static void tu6_9str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1 = 0;
    fd_set  rset;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];

    FD_ZERO(&rset);
    while (1)
    {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        maxfdp1 = max(fileno(fp), sockfd) + 1;
        my_select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset))
        {
            if (my_readline(sockfd, recvline, MAXLINE) == 0)
            {
                err_quit("str_cli: server terminated prematurely");
            }

            my_fputs(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset))
        {
             if (my_fgets(sendline, MAXLINE, fp) == NULL)
             {
                 return ;
             }

             my_writen(sockfd, sendline, strlen(sendline));
        }
    }
}


//6.7 p137 tu6_13
void tu6_13str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1 = 0;
    int     stdineof = 0;
    fd_set  rset ;
    char    buf[MAXLINE];
    int     n = 0;

    stdineof = 0;
    FD_ZERO(&rset);

    while(1)
    {
         if (stdineof == 0)
         {
             FD_SET(fileno(fp), &rset);             //这里不太明白
         }

         FD_SET(sockfd, &rset);
         maxfdp1 = max(fileno(fp), sockfd) + 1;

         my_select(maxfdp1, &rset, NULL, NULL, NULL);

         if (FD_ISSET(sockfd, &rset))
         {
             if ((n = my_read(sockfd, buf, MAXLINE)) == 0)
             {
                  if (stdineof == 1)
                  {
                      return ;
                  }
                  else
                  {
                       err_quit("str_cli:server terminated prematurely");
                  }
             }
        }
         my_writen(fileno(stdout), buf, n);


        if (FD_ISSET(fileno(fp), &rset))
        {
            if ((n = my_read(fileno(fp), buf, MAXLINE)) == 0)
            {
                 stdineof = 1;
                 my_shutdown(sockfd, SHUT_WR);
                 FD_CLR(fileno(fp), &rset);
                 continue;
            }

            my_writen(sockfd, buf, n);
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd = 0;
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        err_quit("usage: ./tcpcli <IPaddress>");
    }

    sockfd = my_socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    my_inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    my_connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    tu6_9str_cli(stdin, sockfd);

    return 0;
}
