#include "fcgircd.h"

void do_index(struct fcgircd_state *state, char *query_string) {
    print_file(HEADER_PATH);
    if(state->connected == FCGIRCD_FALSE) {
        print_file(CONNECT_FORM_PATH);
    }
    //now we have state struct..  which will certainly evolve as we go.
    //Right now it's just state->connected = bool and state->uid = unique id per visitor.
    print_file(FOOTER_PATH);
}