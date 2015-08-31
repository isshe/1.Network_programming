/*
 * =====================================================================================
 *
 *       Filename:  test_main.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年08月03日 星期一 11時53分25秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_

#include <sys/utsname.h>
#include "my.h"
#include "addrinfo.h"
#include <netdb.h>
#include <arpa/nameser.h>
#include <resolv.h>


#define SENDRATE 5		//发送频率，5秒一次


void recv_all(int, socklen_t);
void send_all(int, SA *, socklen_t);	//SA要注意
int udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);
int Udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenptr);

int mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);
void Mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);

int Family_to_level(int family);
int family_to_level(int family);

unsigned int if_nametoindex(const char *name);
unsigned int If_nametoindex(const char *name);

int getaddrinfo(const char *hostname, const char *servname,
			const struct addrinfo *hintsp, struct addrinfo **result);
void freeaddrinfo(struct addrinfo *aihead);

//char *gai_strerror(int err);


#endif
