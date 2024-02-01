#ifndef __ORCAPARSER_H__
#define __ORCAPARSER_H__
typedef struct Element {
    double E;
    double Real;
    double Imag;
    int Root;
    int Spin;
    int Ms;
} Element;
extern void parse_element(char *buffer, Element *el);
typedef struct State {
    long n;
    struct Element elements[];
} State;
#endif
