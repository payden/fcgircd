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
#define INITIAL_HEADER_LENGTH 512
#define UID_COOKIE "uid"
#define UID_LENGTH 50
#define MAX_COOKIES 50

#ifdef	__cplusplus
extern "C" {
#endif

    char *response_headers;
    int response_headers_sz;
    char *cookie_names[MAX_COOKIES];
    char *cookie_values[MAX_COOKIES];
    int cookie_count;
    char *generate_uid(void);
    char *get_cookie(char *cookie_name);
    void init_cookies(void);
    void free_cookies(void);
    void add_response_header(char *header);
    void set_cookie(char *name, char *value);
    void set_on_empty_identifer(void);
    void init_response_headers(void);
    void init_memcached(memcached_st *mem);
    void print_file(char *path);
    void route_request(void);
    void output_headers(void);
    void do_index(char *query_string);


#ifdef	__cplusplus
}
#endif

#endif	/* FCGIRCD_H */

