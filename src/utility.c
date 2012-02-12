#include "fcgircd.h"

void print_file(char *path) {
    char out_buf[OUTPUT_BUFFER_LENGTH];
    FILE *fp = NULL;
    fp = fopen(path, "r");
    while(!feof(fp)) {
        memset(&out_buf,'\0',OUTPUT_BUFFER_LENGTH);
        fread(&out_buf, OUTPUT_BUFFER_LENGTH-1, 1, fp);
        printf("%s",out_buf);
    }
    fclose(fp);
}

void output_headers(void) {
    printf("Content-type: text/html\n\n");
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
