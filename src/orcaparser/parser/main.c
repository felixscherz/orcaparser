#include <Python.h>
#include <orcaparser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PyObject *element_to_dict(Element *el) {
    return Py_BuildValue("{s:f, s:f, s:f, s:i, s:i, s:i}", "E", el->E, "Real",
                         el->Real, "Imag", el->Imag, "Root", el->Root, "Spin",
                         el->Spin, "Ms", el->Ms);
}

static PyObject *elements_to_dict(Element *els, int len) {
    PyObject *elements_list = PyList_New(0);
    for (int i = 0; i < len; i++) {
        PyObject *dict = element_to_dict(&els[i]);
        PyList_Append(elements_list, dict);
    }
    return elements_list;
}

static PyObject *state_to_dict(State *state) {
    PyObject *elements_list = elements_to_dict(*state->elements, state->n_el);
    return Py_BuildValue("{s:i, s:O}", "n", state->n, "elements",
                         elements_list);
}

static PyObject *states_to_list(State *states, int len) {
    PyObject *states_list = PyList_New(0);
    for (int i = 0; i < len; i++) {
        PyObject *dict = state_to_dict(&states[i]);
        PyList_Append(states_list, dict);
    }
    return states_list;
}

static PyObject *method_parse_states(PyObject *self, PyObject *args) {
    char *filename = NULL;
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("couldn't open file\n");
        return NULL;
    }
    printf("opened %s\n", filename);

    char buffer[1024];
    char start[1024];
    strcpy(start, "Eigenvectors of the SOC matrix:\n");
    while ((fgets(buffer, sizeof(buffer), fp)) != NULL) {
        if (strcmp(start, buffer) == 0) {
            break;
        };
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    State *states_p;
    int no_of_states = parse_states(fp, &states_p);

    fclose(fp);

    return states_to_list(states_p, no_of_states);
}

static PyMethodDef OrcaparserMethods[] = {
    {"parse_states", method_parse_states, METH_VARARGS,
     "Python interface for parse states"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef orcaparsermodule = {
    PyModuleDef_HEAD_INIT, "parser", "Orcaparser python interface", -1,
    OrcaparserMethods};

/* static PyMethodDef ParentMethods[] = { */
/*     {NULL, NULL, 0, NULL}}; */
/**/
/* static struct PyModuleDef parentmodule = { */
/*     PyModuleDef_HEAD_INIT, "orcaparser", "Orcaparser python interface", -1,
 */
/*     ParentMethods}; */
PyMODINIT_FUNC PyInit_parser(void) { // turns out this name matters
    /* PyObject *parent = PyModule_Create(&parentmodule); */
    return PyModule_Create(&orcaparsermodule);


    /* PyObject *core = PyModule_Create(&orcaparsermodule); */
    /* return core; */

    /* PyModule_AddObject(parent, "core", core); */
    /**/
    /**/
    /* return parent; */
    /* return PyModule_Create(&orcaparsermodule); */
    /* return PyModule_Create(&orcaparsermodule); */
}
