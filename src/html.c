#include "fcgircd.h"

void do_index(char *query_string) {
    char *test = get_cookie("uid");
    print_file(HEADER_PATH);
    if(test!=NULL) {
        printf("Cookie uid: %s<br>Cookie count: %d\n",test,cookie_count);
        free(test);
    }
    print_file(FOOTER_PATH);
}