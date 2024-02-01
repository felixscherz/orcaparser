#include <orcaparser.h>
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_element(Element *element) {
    printf("E = %f; ", element->E);
    printf("Real = %f; ", element->Real);
    printf("Imag = %f; ", element->Imag);
    printf("Root = %d; ", element->Root);
    printf("Spin = %d; ", element->Spin);
    printf("Ms = %d; ", element->Ms);
    printf("\n");
}

void print_state(State *state) { printf("state no %ld\n", state->n); }

int parse_states(FILE *file, State *states) {
    /* initially parsing a state line  */
    SM_state state = STATE_LINE;
    SM_transition transition = START;
    char buffer[1024];
    char *check = "\n";
    int state_no;
    int el_counter = 0;
    Element *elements;
    elements = (Element *)malloc(8 * sizeof(Element));
    State *current_state;
    current_state = malloc(sizeof(State));
    int no_of_states = 0;

    fgets(buffer, sizeof(buffer), file);
    while (strcmp(fgets(buffer, sizeof(buffer), file), check) != 0) {
        if (is_state(buffer) == 0) {
            state = BLOCK_LINE;
        } else {
            if (state == BLOCK_LINE) {
                /* going from block to state -> end of a state */
                memcpy(&states[state_no], current_state,
                       sizeof(*current_state));
            }
            state = STATE_LINE;
        }
        switch (state) {
            case STATE_LINE: {
                no_of_states++;
                printf("parsing new state ");
                char *c;
                c = strtok(buffer, ":");
                c = strtok(c, " ");
                c = strtok(NULL, " ");
                char *ptr;
                state_no = strtol(c, &ptr, 10);
                printf("no: %d\n", state_no);
                current_state->n = state_no;
                el_counter = 0;
                break;
            }
            case BLOCK_LINE: {
                Element element = elements[el_counter];
                parse_element(buffer, &element);
                print_element(&element);
                break;
            }
        }
    }
    /* copy last state */
    memcpy(&states[state_no], current_state, sizeof(*current_state));

    printf("states ended\n");
    return no_of_states;
}

int main(int argc, char **argv) {
    printf("received %d arguments\n", argc);

    char *filename;

    if (argc > 1) {
        printf("received %s\n", argv[1]);
        filename = argv[1];
    } else {
        printf("pass filename as argument\n");
        return 0;
    }

    FILE *file;
    filename = "ch2o_soc_tddft.out";
    file = fopen(filename, "r");
    if (file == 0) {
        printf("something wrong with opening file\n");
        return 1;
    }
    char buffer[1024];
    char start[1024];
    strcpy(start, "Eigenvectors of the SOC matrix:\n");
    while ((fgets(buffer, sizeof(buffer), file)) != NULL) {
        if (strcmp(start, buffer) == 0) {
            printf("found start\n");
            break;
        };
    }
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);

    int max_num_states = 50;
    int max_num_elements = 10;
    State *states = malloc(max_num_states * sizeof(State));
    Element *elements;
    for (int i = 0; i < max_num_states; i++) {
        elements = malloc(max_num_elements * sizeof(Element));
        memcpy(states[i].elements, elements, sizeof(*elements));
    }

    int no_of_states = parse_states(file, states);
    fclose(file);

    int i = 0;
    for (int i = 0; i < no_of_states+1; i++) {
        print_state(&states[i]);
    }

    return 0;
}
