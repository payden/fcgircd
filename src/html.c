#include "fcgircd.h"

void do_index(char *uri, char *query_string) {
    print_file(HEADER_PATH);
    print_file(FOOTER_PATH);
}