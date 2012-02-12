#include <sys/time.h>

#include "fcgircd.h"


char *generate_uid(void) {
    struct timeval tv;
    unsigned int seed;
    char *uid = NULL;
    int x;
    unsigned char ch = 0;
    uid = (char *)malloc(sizeof(char) * UID_LENGTH);
    gettimeofday(&tv, NULL);
    seed = tv.tv_sec * tv.tv_usec;
    srand(seed);
    //ranges 0-9 48-57, A-Z 65-90, a-z 97-122 inclusive.
    for(x=0;x<UID_LENGTH;x++) {
        while( (ch < 48 || ch > 57) && (ch < 65 || ch > 90) && (ch < 97 || ch > 122) ) {
            ch = (unsigned char) (rand() % 74) + 48;
        }
        *uid = ch;
        uid++;
        ch = 0;
    }
    return uid - UID_LENGTH;
}

void free_cookies(void) {
    int x;
    for(x=0;x<cookie_count;x++) {
        if(cookie_names[x]!=NULL) {
            free(cookie_names[x]);
            cookie_names[x] = NULL;
        }
        if(cookie_values[x]!=NULL) {
            free(cookie_values[x]);
            cookie_values[x] = NULL;
        }
    }
}

void init_cookies(void) {
    char *env_cookie = getenv("HTTP_COOKIE");
    char *my_cookie = NULL;
    char *cookies = NULL;
    char *tmp_name = NULL;
    char *tmp_value = NULL;
    char *cookie_strs[MAX_COOKIES];
    int i;
    cookie_count = 0;
    if(env_cookie == NULL) {
        return;
    }
    my_cookie = strdup(env_cookie);
    if(my_cookie == NULL) {
        syslog(LOG_NOTICE, "Unable to allocate memory in get_cookie(). Exiting..\n");
        exit(1);
    }
    for(cookies=strtok(my_cookie,"; "), i=0;cookies!=NULL;cookies=strtok(NULL, "; "), i++) {
        if(strlen(cookies)!=0) {
            cookie_strs[i] = (char *) malloc(sizeof (char) * strlen(cookies) + 1);
            memset(cookie_strs[i], '\0', strlen(cookies)+1);
            strncpy(cookie_strs[i], cookies, strlen(cookies));
            cookie_count++;
        }
    }
    for(i=0;i<cookie_count;i++) {
        tmp_name = strtok(cookie_strs[i], "=");
        if(tmp_name==NULL) { continue; }
        cookie_names[i] = (char *)malloc(sizeof(char) * strlen(tmp_name)+1);
        if(cookie_names[i]==NULL) {
            syslog(LOG_NOTICE, "Unable to allocate memory in init_cookies().  Exiting..\n");
            exit(1);
        }
        memset(cookie_names[i],'\0',strlen(tmp_name)+1);
        strncpy(cookie_names[i], tmp_name, strlen(tmp_name));
        tmp_value = strtok(NULL, "=");
        if(tmp_value == NULL) { continue; }
        cookie_values[i] = (char *)malloc(sizeof(char) * strlen(tmp_value)+1);
        if(cookie_values[i]==NULL) {
            syslog(LOG_NOTICE, "Unable to allocate memory in init_cookies().  Exiting..\n");
            exit(1);
        }
        memset(cookie_values[i],'\0',strlen(tmp_value)+1);
        strncpy(cookie_values[i], tmp_value, strlen(tmp_value));
        free(cookie_strs[i]);
    }
    free(my_cookie);
}

char *get_cookie(char *name) {
    int x;
    for(x=0;x<cookie_count;x++) {
        if(strcmp(cookie_names[x], name)==0) {
            return cookie_values[x]!=NULL ? strdup(cookie_values[x]) : NULL;
        }
    }
    return NULL;
}

void set_cookie(char *name, char *value) {
    size_t str_sz = strlen(name) + strlen(value) + 20; //should only need 15 extra, but 20 just in case
    char *set_cookie_str = (char *)malloc(sizeof(char) * str_sz);
    snprintf(set_cookie_str,str_sz,"Set-Cookie: %s=%s;\n",name, value);
    add_response_header(set_cookie_str);
    free(set_cookie_str);
}

void set_on_empty_identifier(void) {
    char *uid_cookie = NULL;
    char *generated_uid = NULL;
    if((uid_cookie = get_cookie(UID_COOKIE))==NULL) {
        generated_uid = generate_uid();
        set_cookie(UID_COOKIE, generated_uid);
        free(generated_uid);
    }
    else {
        //Found unique id, just free it for now.
        free(uid_cookie);
    }
}

void print_file(char *path) {
    char out_buf[OUTPUT_BUFFER_LENGTH];
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if(fp == NULL) {
        printf("Unable to open file in print_file: %s\n",path);
        exit(1);
    }
    while(!feof(fp)) {
        memset(&out_buf,'\0',OUTPUT_BUFFER_LENGTH);
        fread(&out_buf, OUTPUT_BUFFER_LENGTH-1, 1, fp);
        printf("%s",out_buf);
    }
    fclose(fp);
}

void add_response_header(char *header) {
    char *new_headers = NULL;
    if(strlen(header) >= 512) {
        syslog(LOG_NOTICE, "Header too large in add_response_header(), refusing to add it.");
        return;
    }
    //greater than or equal to because even if they're equal we don't have room for null byte.
    if(strlen(header) + strlen(response_headers) >= response_headers_sz) {
        response_headers_sz += 512; //allocate in 512 byte blocks
        new_headers = (char *)malloc(sizeof(char) * response_headers_sz);
        if(new_headers == NULL) {
            syslog(LOG_NOTICE, "Unable to allocate memory in add_response_headers().  Exiting..\n");
            exit(1);
        }
        memset(new_headers,'\0',response_headers_sz);
        strncpy(new_headers,response_headers,strlen(response_headers));
        syslog(LOG_NOTICE, "Increasing size of response_headers to %d bytes, we ran out of room.", response_headers_sz);
        free(response_headers);
        response_headers = new_headers;
        new_headers = NULL;
    }
    strncat(response_headers, header, strlen(header));
}

void init_response_headers() {
    const char *content_type_header = "Content-type: text/html\n";
    response_headers = (char *)malloc(sizeof(char)*INITIAL_HEADER_LENGTH);
    if(response_headers == NULL) {
        syslog(LOG_NOTICE, "Unable to allocate memory for response headers.  Exiting..\n");
        exit(1);
    }
    memset(response_headers,'\0',INITIAL_HEADER_LENGTH);
    response_headers_sz = INITIAL_HEADER_LENGTH;
    strncpy(response_headers,content_type_header,strlen(content_type_header));
}

void output_headers(void) {
    printf("%s\n",response_headers);
}

void route_request() {
    char *envuri = getenv("REQUEST_URI");
    char *uri = (char *)malloc(sizeof(char)*strlen(envuri)+1);
    char *query_string = NULL;
    if(uri == NULL) {
        syslog(LOG_NOTICE, "Unable to allocate memory for request URI.  Dying.");
        exit(1);
    }
    memset(uri,'\0',strlen(envuri)+1);
    strncpy(uri,envuri,strlen(envuri));
    envuri = NULL;
    if((query_string = strstr(uri,"?"))!=NULL) {
        //replace ? with single NULL byte so we have separate NULL terminated strings that happened to have been allocated at the same time.
        //may need to revisit this, I'm not sure if free() only frees up to the NULL byte or if it's smarter than that.
        memset(query_string,'\0',1);
        query_string++;
    }
    if(strcmp(uri,"/") == 0 || strcmp(uri,"/index.html") == 0) {
        do_index(query_string);
    }
    free(uri);
}

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
        exit(1);
    }
}
