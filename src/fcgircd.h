/* 
 * File:   fcgircd.h
 * Author: payden
 *
 * Created on February 11, 2012, 5:40 PM
 */

#ifndef FCGIRCD_H
#define	FCGIRCD_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmemcached/memcached.h>
#include <fcgi_stdio.h>
#include <syslog.h>

#define PROGRAM_NAME "fcgircd"
#define TEMPLATE_DIR "templates/"
#define HEADER_PATH TEMPLATE_DIR HEADER_HTML
#define FOOTER_PATH TEMPLATE_DIR FOOTER_HTML
#define HEADER_HTML "header.html"
#define FOOTER_HTML "footer.html"
#define OUTPUT_BUFFER_LENGTH 1024

#ifdef	__cplusplus
extern "C" {
#endif

    void init_memcached(memcached_st *mem);
    void print_file(char *path);
    void route_request(void);
    void output_headers(void);
    void do_index(char *query_string);


#ifdef	__cplusplus
}
#endif

#endif	/* FCGIRCD_H */

