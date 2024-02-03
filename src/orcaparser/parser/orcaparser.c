#include "orcaparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { STATE_LINE, BLOCK_LINE } SM_state;
typedef enum {
    STATE_TO_BLOCK,
    BLOCK_TO_BLOCK,
    BLOCK_TO_STATE,
    START
} SM_transition;

int is_state(char *line) {
    if (strncmp(" STATE", line, 4) == 0) {
        return 1;
    };
    return 0;
}

void parse_element(char *values, Element *el) {
    double a;
    char *c = strtok(values, " ");
    a = atof(c);
    el->E = a;
    c = strtok(NULL, " ");
    a = atof(c);
    el->Real = a;
    c = strtok(NULL, " ");
    a = atof(c);
    el->Imag = a;
    /* remove colon */
    c = strtok(NULL, " ");
    c = strtok(NULL, " ");
    a = atof(c);
    el->Root = a;
    c = strtok(NULL, " ");
    a = atof(c);
    el->Spin = a;
    c = strtok(NULL, " ");
    a = atof(c);
    el->Ms = a;
}

void parse_state(char *buffer, State *state) {
    char *c;
    c = strtok(buffer, ":");
    c = strtok(c, " ");
    c = strtok(NULL, " ");
    char *ptr;
    int state_no = strtol(c, &ptr, 10);
    state->n_el = 0;
    state->n = state_no;
    Element **elements_p;
    elements_p = malloc(sizeof(elements_p));
    state->elements = elements_p;
}

void print_element(Element *element) {
    printf("E = %f; ", element->E);
    printf("Real = %f; ", element->Real);
    printf("Imag = %f; ", element->Imag);
    printf("Root = %d; ", element->Root);
    printf("Spin = %d; ", element->Spin);
    printf("Ms = %d; ", element->Ms);
    printf("\n");
}
void print_state(State *state) {
    printf("state no %ld\n", state->n);
    printf("no of el: %d\n", state->n_el);
    printf("pointer to el: %i\n", *(state->elements));
}

int parse_states(FILE *file, State **states_p) {
    /* initially parsing a state line  */
    SM_state state = STATE_LINE;
    SM_transition transition = START;
    char buffer[1024];
    char *check = "\n";
    int state_no;
    int el_counter = 0;
    int no_of_states = -1;
    *states_p = malloc(sizeof(State));

    while (strcmp(fgets(buffer, sizeof(buffer), file), check) != 0) {
        if (is_state(buffer) == 0) {
            state = BLOCK_LINE;
        } else {
            if (state == BLOCK_LINE) {
                /* going from block to state -> end of a state */
            }
            state = STATE_LINE;
        }
        switch (state) {
            case STATE_LINE: {
                no_of_states++;
                *states_p =
                    realloc(*states_p, sizeof(State) * (no_of_states + 1));
                parse_state(buffer, &(*states_p)[no_of_states]);
                el_counter = 0;
                (*states_p)[no_of_states].n_el = el_counter;
                Element *elements = malloc(sizeof(Element) * (el_counter + 1));
                *(*states_p)[no_of_states].elements = elements;
                break;
            }
            case BLOCK_LINE: {
                *(*states_p)[no_of_states].elements =
                    realloc(*(*states_p)[no_of_states].elements, sizeof(Element) * (el_counter + 1));
                (*states_p)[no_of_states].n_el++;
                parse_element(buffer, &(*(*states_p)[no_of_states].elements)[el_counter]);
                el_counter++;
                break;
            }
        }
    }
    return no_of_states;
}
