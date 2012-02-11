#include <fcgi_stdio.h>
#include <libmemcached/memcached.h>

#include "fcgircd.h"

int main(void) {
    int count = 0;
    struct memcached_st *mem;
    init_memcached(mem);
    printf("Successfully connected to memcached");
    
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\n\nHello world again..(%d)\n",++count);
    }
    return 0;
}
