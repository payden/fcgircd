#include "fcgircd.h"

void init_epoll(int *epfd, struct epoll_event **events) {
    *events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EVENTS);
    if(*events == NULL) {
        syslog(LOG_NOTICE, "Out of memory on malloc for epoll_events");
        exit(1);
    }
    *epfd = epoll_create(MAX_EVENTS);
    if(*epfd == -1) {
        syslog(LOG_NOTICE, "Unable to epoll_create(), exiting..");
        exit(1);
    }
}
