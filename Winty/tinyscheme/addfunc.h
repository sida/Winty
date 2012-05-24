#ifndef ADDFUND_H
#define ADDFUND_H

#include "scheme-private.h"
#define _SCHEME_SOURCE

//pointer scm_LoadLibrary(scheme *sc, pointer args);
//pointer scm_call_Library(scheme *sc, pointer args);
pointer scm_getViewhWnd(scheme *sc, pointer args);
pointer scm_getViewhDc(scheme *sc, pointer args);
pointer scm_system(scheme *sc, pointer args);
pointer scm_getWmCommand(scheme *sc, pointer args);

//pointer scm_malloc(scheme *sc, pointer args);
//pointer scm_free(scheme *sc, pointer args);
//pointer scm_set_long(scheme *sc, pointer args);
//pointer scm_get_long(scheme *sc, pointer args);
//pointer scm_set_uchar(scheme *sc, pointer args);
//pointer scm_get_uchar(scheme *sc, pointer args);
//pointer scm_console_input_port(scheme *sc, pointer args);
//pointer scm_console_output_port(scheme *sc, pointer args);

//pointer scm_set_input_port_file(scheme *sc, pointer args);
//pointer scm_set_output_port_file(scheme *sc, pointer args);

//typedef struct _dllargs
//{
//	int kind;
//	void *val;
//} DLLARGS;

#endif
