/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : noblock_select_strcli.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/22
* Description  : 非阻塞I/O的客户端程序
* Function list: 1.str_cli();
*                2.
*                3.
* History      :
\*==============================================================================*/

#include "my.h"

char *gf_time(void)
{
    struct  timeval     tv;
    time_t  t;
    static  char        str[30];
    char    *ptr = NULL;

    if (gettimeofday(&tv, NULL) < 0)
    {
         err_sys("gettimeofday error");
    }

    t = tv.tv_sec ;         //POSIX says tv.tv_secis time_t; some BSDs don't agree
    ptr = ctime(&t);

    strcpy(str, &ptr[11]);          //???

    snprintf(str+8, sizeof(str) - 8, ".%06ld", tv.tv_usec);

    return(str);
}

void tu16_3str_cli(FILE *fp, int sockfd)
{
    int     maxfdp1 = 0;
    int     val = 0;
    int     stdineof = 0;
    ssize_t n ;
    ssize_t nwritten ;
    fd_set  rset;           //r = read; w = write;
    fd_set  wset;
    char    to[MAXLINE] = {0};
    char    fr[MAXLINE] = {0};
    char    *toiptr = NULL;
    char    *tooptr = NULL;
    char    *friptr = NULL;
    char    *froptr = NULL;

    //设置套接字sockfd,STDIN_FILENO, STDOUT_FILENO为非阻塞
    val = my_fcntl(sockfd, F_GETFL, 0);
    my_fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val = my_fcntl(STDIN_FILENO, F_GETFL, 0);
    my_fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = my_fcntl(STDOUT_FILENO, F_GETFL, 0);
    my_fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to;           //initialize buffer pointers
    friptr = froptr = fr;

    stdineof = 0;

    maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
    while(1)
    {
         FD_ZERO(&rset);
         FD_ZERO(&wset);

         if (stdineof == 0 && toiptr < &to[MAXLINE])        //
         {
             //从标准输入读取(read())数据到空闲的缓冲区
             //稍后用write()写到fd, 以发送给服务器
             FD_SET(STDIN_FILENO, &rset);
         }

         if (friptr < &fr[MAXLINE])
         {
              //从套接字读取(read())数据到空闲的缓冲区,
              //稍后用write() 写到标准输出, 以显示
              FD_SET(sockfd, &rset);
         }

         if (toiptr != tooptr)
         {
             //缓冲区中有待发往服务器的数据
             //用write()写到fd, 以发往服务器
             FD_SET(sockfd, &wset);
         }

         if (friptr != froptr)
         {
             //缓冲区中有待发往标准输出的数据;
             //用write()写到标准输出, 以显示
              FD_SET(STDOUT_FILENO, &wset);
         }

         //select()
         my_select(maxfdp1, &rset, &wset, NULL, NULL);

         if (FD_ISSET(STDIN_FILENO, &rset))
         {
              //键盘有数据输入
              if ( (n = read(STDIN_FILENO, toiptr,
                              &to[MAXLINE] - toiptr)) < 0 )
              {
                  if (errno != EWOULDBLOCK)
                  {
                      err_sys("read error on stdin");
                  }
              }
              else if (0 == n)
              {
                   fprintf(stderr, "%s: EOF on stdin\n", gf_time());
                   stdineof = 1;        //读到EOF, 所以置1!
                   if (tooptr == toiptr)
                   {
                        my_shutdown(sockfd, SHUT_WR);       //send FIN
                   }
              }
              else
              {
                  fprintf(stderr, "%s: stdin read %ld bytes from stdin\n",
                          gf_time(), n);
                  toiptr += n;      //just read

                  //有数据准备好发送了, 马上写到套接字, 以发送到服务器?
                  FD_SET(sockfd, &wset);        //???
              }

         } //if for stdin read

         //个人理解:数据从sockfd来, 要读取放到接收缓冲区
         if (FD_ISSET(sockfd, &rset))
         {
              if ( (n = read(sockfd, friptr,
                              &fr[MAXLINE] - friptr)) < 0 )
              {
                  if (errno != EWOULDBLOCK)
                  {
                      err_sys("read error on socket");
                  }
              }
              else if (0 == n)
              {
                   fprintf(stderr, "%s: EOF on socket\n", gf_time());

                    //前面已经读到EOF了, 没有数据要发送了给服务器了,
                    //所以 n = 0 代表数据接收完, 故可以退出
                   if (stdineof)
                   {
                       return ;         //normal termination
                   }
                   else
                   {
                       err_quit("str_cli: server terminated prematurely");
                   }
              }
              else
              {
                   fprintf(stderr, "%s: sockfd read %ld bytes from socket\n",
                           gf_time(), n);
                   friptr += n;

                   //接收到数据, 马上尝试写到标准输出, 以显示?
                   FD_SET(STDOUT_FILENO, &wset); //??? try to write below
              }
         }// if for sockfd read

         if (FD_ISSET(STDOUT_FILENO, &wset)
             && ((n = friptr - froptr) > 0))
         {
             //这个write()实现了显示到屏幕!!!没有什么printf()!
              if ( (nwritten = write(STDOUT_FILENO, froptr, n)) < 0 )
              {
                   if (errno != EWOULDBLOCK)
                   {
                       err_sys("write error to stdout");
                   }
              }
              else
              {
                   fprintf(stderr, "%s: stdout wrote %ld byte to stdout\n",
                           gf_time(), nwritten);
                   froptr += nwritten;
                   if (froptr == friptr)     //全部数据写完, 都重置
                   {
                       froptr = friptr = fr; //back to beginning of buf
                   }
              }
         } // if for STDOUT_FILENO write

         if (FD_ISSET(sockfd, &wset)
             &&((n = toiptr - tooptr) > 0))
         {
              if ( (nwritten = write(sockfd, tooptr, n)) < 0 )
              {
                  if (EWOULDBLOCK != errno)
                  {
                      err_sys("write error to socket");
                  }
              }
              else
              {
                   fprintf(stderr, "%s: sockfd wrote %ld bytes to socket\n",
                           gf_time(), nwritten);

                   tooptr += nwritten;

                   if (tooptr == toiptr)
                   {
                        toiptr = tooptr = to;

                        // 为"1"说明往后没有数据了, 所以可以关闭了
                        if (stdineof)
                        {
                            my_shutdown(sockfd, SHUT_WR);
                        }
                   }
              }
         } //if for sockfd write
    }
}
