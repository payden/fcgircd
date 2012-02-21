#include "fcgircd.h"

void init_epoll(int *epfd) {
    *epfd = epoll_create(MAX_EVENTS);
    if(*epfd == -1) {
        syslog(LOG_NOTICE, "Unable to epoll_create(), exiting..");
        exit(1);
    }
}
