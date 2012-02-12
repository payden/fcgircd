#include <libmemcached/memcached.h>

#include "fcgircd.h"

int main(void) {
    struct memcached_st *mem;
    struct fcgircd_state *state;
    char *current_uid = NULL;
    openlog(PROGRAM_NAME, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);
    init_memcached(&mem);
    
    while (FCGI_Accept() >= 0) {
        init_cookies();
        init_response_headers();
        current_uid = set_on_empty_identifier();
        state = populate_state_from_memcached(mem, current_uid);
        output_headers();
        route_request(state);
        save_state_to_memcached(mem, state);
        free_cookies();
        if(state!=NULL) { free(state); }
        free(current_uid);
        free(response_headers);
    }
    memcached_free(mem);
    closelog();
    return 0;
}
