/* include udp_client */
#include	"my.h"
#include "test_main.h"
#include "gai_hdr.h"


int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("udp_client error for %s, %s: %s",
				 host, serv, gai_strerror(n));
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break;		/* success */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final socket() */
		err_sys("udp_client error for %s, %s", host, serv);

	*saptr = my_malloc(res->ai_addrlen);
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_client */

int
Udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenptr)
{
	return(udp_client(host, serv, saptr, lenptr));
}

/////////
int
getaddrinfo(const char *hostname, const char *servname,
			const struct addrinfo *hintsp, struct addrinfo **result)
{
	int					rc, error, nsearch;
	char				**ap, *canon;
	struct hostent		*hptr;
	struct search		search[3], *sptr;
	struct addrinfo		hints, *aihead, **aipnext;

	/*
	 * If we encounter an error we want to free() any dynamic memory
	 * that we've allocated.  This is our hack to simplify the code.
	 */
#define	error(e) { error = (e); goto bad; }

	aihead = NULL;		/* initialize automatic variables */
	aipnext = &aihead;
	canon = NULL;

	if (hintsp == NULL) {
		bzero(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
	} else
		hints = *hintsp;		/* struct copy */

		/* 4first some basic error checking */
	if ( (rc = ga_echeck(hostname, servname, hints.ai_flags, hints.ai_family,
						 hints.ai_socktype, hints.ai_protocol)) != 0)
		error(rc);

#ifdef	UNIXdomain
		/* 4special case Unix domain first */
	if (hostname != NULL &&
		(strcmp(hostname, "/local") == 0 || strcmp(hostname, "/unix") == 0) &&
		(servname != NULL && servname[0] == '/'))
		return(ga_unix(servname, &hints, result));
#endif
/* end ga1 */

/* include ga3 */
		/* 4remainder of function for IPv4/IPv6 */
	nsearch = ga_nsearch(hostname, &hints, &search[0]);
	for (sptr = &search[0]; sptr < &search[nsearch]; sptr++) {
#ifdef	IPv4
			/* 4check for an IPv4 dotted-decimal string */
		if (isdigit(sptr->host[0])) {
			struct in_addr	inaddr;

			if (inet_pton(AF_INET, sptr->host, &inaddr) == 1) {
				if (hints.ai_family != AF_UNSPEC &&
					hints.ai_family != AF_INET)
					error(EAI_ADDRFAMILY);
				if (sptr->family != AF_INET)
					continue;		/* ignore */
				rc = ga_aistruct(&aipnext, &hints, &inaddr, AF_INET);
				if (rc != 0)
					error(rc);
				continue;
			}
		}
#endif
	
#ifdef	IPv6
			/* 4check for an IPv6 hex string */
		if ((isxdigit(sptr->host[0]) || sptr->host[0] == ':') &&
			(strchr(sptr->host, ':') != NULL)) {
			struct in6_addr	in6addr;

			if (inet_pton(AF_INET6, sptr->host, &in6addr) == 1) {
				if (hints.ai_family != AF_UNSPEC &&
					hints.ai_family != AF_INET6)
					error(EAI_ADDRFAMILY);
				if (sptr->family != AF_INET6)
					continue;		/* ignore */
				rc = ga_aistruct(&aipnext, &hints, &in6addr, AF_INET6);
				if (rc != 0)
					error(rc);
				continue;
			}
		}
#endif
/* end ga3 */
/* include ga4 */
			/* 4remainder of for() to look up hostname */
		if ((_res.options & RES_INIT) == 0)
			res_init();			/* need this to set _res.options */

		if (nsearch == 2) {
#ifdef	IPv6
			_res.options &= ~RES_USE_INET6;
#endif
			hptr = gethostbyname2(sptr->host, sptr->family);
		} else {
#ifdef  IPv6
			if (sptr->family == AF_INET6)
				_res.options |= RES_USE_INET6;
			else
				_res.options &= ~RES_USE_INET6;
#endif
			hptr = gethostbyname(sptr->host);
		}
		if (hptr == NULL) {
			if (nsearch == 2)
				continue;	/* failure OK if multiple searches */

			switch (h_errno) {
				case HOST_NOT_FOUND:	error(EAI_NONAME);
				case TRY_AGAIN:			error(EAI_AGAIN);
				case NO_RECOVERY:		error(EAI_FAIL);
				case NO_DATA:			error(EAI_NODATA);
				default:				error(EAI_NONAME);
			}
		}
	
			/* 4check for address family mismatch if one specified */
		if (hints.ai_family != AF_UNSPEC && hints.ai_family != hptr->h_addrtype)
			error(EAI_ADDRFAMILY);

			/* 4save canonical name first time */
		if (hostname != NULL && hostname[0] != '\0' &&
			(hints.ai_flags & AI_CANONNAME) && canon == NULL) {
			if ( (canon = strdup(hptr->h_name)) == NULL)	
				error(EAI_MEMORY);
		}
	
			/* 4create one addrinfo{} for each returned address */
		for (ap = hptr->h_addr_list; *ap != NULL; ap++) {
			rc = ga_aistruct(&aipnext, &hints, *ap, hptr->h_addrtype);
			if (rc != 0)
				error(rc);
		}
	}
	if (aihead == NULL)
		error(EAI_NONAME);		/* nothing found */
/* end ga4 */

/* include ga5 */
		/* 4return canonical name */
	if (hostname != NULL && hostname[0] != '\0' &&
		hints.ai_flags & AI_CANONNAME) {
		if (canon != NULL)
			aihead->ai_canonname = canon;	/* strdup'ed earlier */
		else {
			if ( (aihead->ai_canonname = strdup(search[0].host)) == NULL)
				error(EAI_MEMORY);
		}
	}

		/* 4now process the service name */
	if (servname != NULL && servname[0] != '\0') {
		if ( (rc = ga_serv(aihead, &hints, servname)) != 0)
			error(rc);
	}

	*result = aihead;	/* pointer to first structure in linked list */
	return(0);

bad:
	freeaddrinfo(aihead);	/* free any alloc'ed memory */
	return(error);
}
/* end ga5 */


void
freeaddrinfo(struct addrinfo *aihead)
{
	struct addrinfo	*ai, *ainext;

	for (ai = aihead; ai != NULL; ai = ainext) {
		if (ai->ai_addr != NULL)
			free(ai->ai_addr);		/* socket address structure */

		if (ai->ai_canonname != NULL)
			free(ai->ai_canonname);

		ainext = ai->ai_next;	/* can't fetch ai_next after free() */
		free(ai);				/* the addrinfo{} itself */
	}
}
/* end freeaddrinfo */

/*
char *
gai_strerror(int err)
{
	switch (err) {
		case EAI_ADDRFAMILY:return("address family for host not supported");
		case EAI_AGAIN:		return("temporary failure in name resolution");
		case EAI_BADFLAGS:	return("invalid flags value");
		case EAI_FAIL:		return("non-recoverable failure in name resolution");
		case EAI_FAMILY:	return("address family not supported");
		case EAI_MEMORY:	return("memory allocation failure");
		case EAI_NODATA:	return("no address associated with host");
		case EAI_NONAME:	return("host nor service provided, or not known");
		case EAI_SERVICE:	return("service not supported for socket type");
		case EAI_SOCKTYPE:	return("socket type not supported");
		case EAI_SYSTEM:	return("system error");
		default:			return("unknown getaddrinfo() error");
	}
}
*/
