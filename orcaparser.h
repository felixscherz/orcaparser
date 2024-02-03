#ifndef __ORCAPARSER_H__
#define __ORCAPARSER_H__
#include <stdio.h>
typedef struct Element {
    double E;
    double Real;
    double Imag;
    int Root;
    int Spin;
    int Ms;
} Element;
typedef struct State {
    long n;
    int n_el;
    Element **elements;
} State;
extern void print_element(Element *element);
extern void print_state(State *state);
extern int parse_states(FILE *file, State **states);
#endif
