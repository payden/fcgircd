#include <stdio.h>
#include <libmemcached/memcached.h>
#include "fcgircd.h"

void init_memcached(memcached_st *mem) {
    const char memcached_server[] = "127.0.0.1";
    in_port_t memcached_port = 11211;
    memcached_return_t ret;
    mem = memcached_create(NULL);
    if(mem == NULL) {
        fprintf(stderr, "Unable to create memcached_st.  Exiting...\n");
        exit(1);
    }
    ret = memcached_server_add(mem, memcached_server, memcached_port);
    if(ret != MEMCACHED_SUCCESS) {
        fprintf(stderr, "Unable to connect to memcached server on %s:%d.  Exiting...\n",memcached_server,memcached_port);
        exit(2);
    }
}
