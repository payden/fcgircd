#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "fcgircd.h"

void attempt_connection(char *host, char *port, struct fcgircd_state *state) {
	struct addrinfo *ainfo, *p, hints;
	struct epoll_event event;
	int sockfd;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if((getaddrinfo(host, port, &hints, &ainfo)) != 0) {
		syslog(LOG_NOTICE, "Unable to connect to %s:%s in attempt_connection()", host, port);
		return;
	}
	for(p = ainfo;p!=NULL;p=p->ai_next) {
		if((sockfd=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1) {
			syslog(LOG_NOTICE, "socket() failed");
			continue;
		}
		if(connect(sockfd, p->ai_addr, p->ai_addrlen)==-1) {
			close(sockfd);
			syslog(LOG_NOTICE, "connect() failed");
			continue;
		}
	}
	if(p == NULL) {
		syslog(LOG_NOTICE, "Looped through addrinfos with no result");
		return;
	}
	freeaddrinfo(ainfo);
	//update state structure with our new socket descriptor
	state->fd = sockfd;
	//fuck, want to store ptr to state structure in event.data.ptr so when event fires the child process has it.
	//problem is, this is allocated from memcached currently and goes away after each request.  Need to noodle this.
	//Perhaps I the shared memory segment bigger and store state structures in shared mem.  But then we need to garbage collect on them periodically
	//blah.. will think about this later

}




