#include "fcgircd.h"

int main(void) {
    struct memcached_st *mem;
    struct fcgircd_state *state;
    struct epoll_event *epoll_events;
    char *current_uid = NULL;
    int epfd;
    openlog(PROGRAM_NAME, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);
    init_memcached(&mem);
    init_shared_mem();
    init_epoll(&epfd);
    if(fork()==0) { //Child process handles network connections with epoll_wait
    	int ret, i, current_fd;
    	while((ret = epoll_wait(epfd, epoll_events, MAX_EVENTS, 100)) >= 0) {
    		if(ret == 0) {
    			//no events
    		}
    		else {
    			for(i=0;i<ret;i++) {
    				state = (struct fcgircd_state *)epoll_events[i].data.ptr;
    				syslog(LOG_NOTICE, "Got event for uid: %s", state->uid);
    			}
    		}
    		if(shared->parent_exited) {
    			syslog(LOG_NOTICE, "My parent exited, I'm bailing..");
    			break;
    		}
    	}
    }
    else { //parent process handles web requests
    	signal(SIGCHLD, SIG_IGN);
    	signal(SIGINT, fcgircd_cleanup);
    	signal(SIGHUP, fcgircd_cleanup);
    	signal(SIGTERM, fcgircd_cleanup);
		while (FCGI_Accept() >= 0) {
			init_cookies();
			init_response_headers();
			current_uid = set_on_empty_identifier();
			state = populate_state_from_memcached(mem, current_uid);
			route_request(mem, state);
			save_state_to_memcached(mem, state);
			free_cookies();
			if(state!=NULL) { free(state); }
			free(current_uid);
			free(response_headers);
		}
    }

	memcached_free(mem);
	closelog();

    return 0;
}
