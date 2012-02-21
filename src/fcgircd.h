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
#include <signal.h>
#include <errno.h>
#include <libmemcached/memcached.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#include <fcgi_stdio.h>
#include <syslog.h>

#define MAX_EVENTS 100
#define PROGRAM_NAME "fcgircd"
#define FCGIRCD_FALSE 0
#define FCGIRCD_TRUE 1
#define TEXT_HTML "text/html"
#define TEXT_JAVASCRIPT "text/javascript"
#define TEXT_CSS "text/css"
#define APPLICATION_JSON "application/json"
#define IMAGE_PNG "image/png"
#define TEMPLATE_DIR "templates"
#define HEADER_PATH TEMPLATE_DIR HEADER_HTML
#define FOOTER_PATH TEMPLATE_DIR FOOTER_HTML
#define FCGIRCD_CSS_PATH TEMPLATE_DIR FCGIRCD_CSS
#define EMBER_JS_PATH TEMPLATE_DIR EMBER_JS
#define FCGIRCD_JS_PATH TEMPLATE_DIR FCGIRCD_JS
#define FCGIRCD_LOGO_PATH TEMPLATE_DIR FCGIRCD_LOGO
#define CONNECT_FORM_PATH TEMPLATE_DIR CONNECT_FORM_HTML
#define HEADER_HTML "/header.html"
#define FOOTER_HTML "/footer.html"
#define FCGIRCD_CSS "/fcgircd.css"
#define JSON_POST "/json"
#define EMBER_JS "/ember-0.9.4.min.js"
#define FCGIRCD_JS "/fcgircd.js"
#define FCGIRCD_LOGO "/logo.png"
#define CONNECT_FORM_HTML "/connect_form.html"
#define OUTPUT_BUFFER_LENGTH 1024
#define INITIAL_HEADER_LENGTH 512
#define UID_COOKIE "uid"
#define UID_LENGTH 50
#define MAX_COOKIES 50
#define ACTION_CONNECT 1
#define ACTION_UNKNOWN 2

#ifdef	__cplusplus
extern "C" {
#endif
    //struct declarations
	struct fcgircd_shared {
		int parent_exited;
		struct epoll_event events[MAX_EVENTS];
	};

    struct fcgircd_state {
        unsigned char uid[UID_LENGTH+1];
        unsigned int connected;
        unsigned int fd;
    };
    
    
    //Global vars
    struct fcgircd_shared *shared;
    char *response_headers;
    int response_headers_sz;
    char *cookie_names[MAX_COOKIES];
    char *cookie_values[MAX_COOKIES];
    int cookie_count;
    
    
    //Function declarations
    void fcgircd_cleanup(int signal);
    struct fcgircd_state *populate_state_from_memcached(struct memcached_st *mem, char *uid);
    void save_state_to_memcached(struct memcached_st *mem, struct fcgircd_state *state);
    char *generate_uid(void);
    char *get_cookie(char *cookie_name);
    char *set_on_empty_identifier(void);
    int get_action(char *input_data);
    int do_action_connect(char *input_data, struct fcgircd_state *state);
    void attempt_connection(char *host, char *port, struct fcgircd_state *state);
    void init_epoll(int *epfd);
    void init_shared_mem(void);
    void init_cookies(void);
    void free_cookies(void);
    void set_content_type(char *content_type);
    void add_response_header(char *header);
    void set_cookie(char *name, char *value);
    void set_on_empty_identifer(void);
    void init_response_headers(void);
    void init_memcached(memcached_st **mem);
    void print_file(char *path);
    void route_request(struct memcached_st *mem, struct fcgircd_state *state);
    void output_headers(void);
    void do_index(struct fcgircd_state *state, char *query_string);
    void handle_json_post(struct memcached_st *mem, struct fcgircd_state *state);

#ifdef	__cplusplus
}
#endif

#endif	/* FCGIRCD_H */

