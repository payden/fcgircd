#include <libmemcached/memcached.h>

#include "fcgircd.h"

int main(void) {
    int count = 0;
    struct memcached_st *mem;
    openlog(PROGRAM_NAME, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);
    init_memcached(mem);
    syslog(LOG_NOTICE, "Successfully initialized connection to memcached\n");
    
    
    while (FCGI_Accept() >= 0) {
        init_cookies();
        init_response_headers();
        set_on_empty_identifier();
        output_headers();
        route_request();
        free_cookies();
        free(response_headers);
    }
    memcached_free(mem);
    closelog();
    return 0;
}
