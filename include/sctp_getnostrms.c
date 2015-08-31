/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : sctp_getnostrms.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/08/18
* Description  :
* Function list: 1.(static)sctp_address_to_associd();
*                2.sctp_get_no_strms();
*                3.
* History      :
\*==============================================================================*/


#include "my.h"
#include <netinet/sctp.h>
#include <sys/socket.h>
#include <sys/types.h>

static sctp_assoc_t
sctp_address_to_associd(int sock_fd, 
						struct sockaddr *sa, 
						socklen_t salen)
{
	struct sctp_paddrparams sp; 
	socklen_t siz;
	
	siz = sizeof(struct sctp_paddrparams);
	bzero(&sp, siz);
	
	memcpy(&sp.spp_address, sa, salen);
	
	sctp_opt_info(sock_fd, 0, 
				  SCTP_PEER_ADDR_PARAMS,
				  &sp, &siz);
				  
	return (sp.spp_assoc_id);
}


int 	
sctp_get_no_strms(int sock_fd, 
				struct sockaddr *to, 
				socklen_t tolen)
{
	socklen_t retsz;
	struct sctp_status status;
	
	retsz = sizeof(status);
	bzero(&status, sizeof(status));
	//这句出问题.
	//听说可以改成:tatus.sstat_assoc_id = sri.sinfo_assoc_id;  		
	status.sstat_assoc_id = sctp_address_to_associd(sock_fd, to, tolen);
	my_getsockopt(sock_fd, IPPROTO_SCTP, SCTP_STATUS, &status, &retsz);
	
	return (status.sstat_outstrms);
}


