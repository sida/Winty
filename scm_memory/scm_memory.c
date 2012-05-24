// scm_memory.cpp : DLL アプリケーションのエントリ ポイントを定義します。
//

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#define _SCHEME_SOURCE
#include "..\Winty\tinyscheme\scheme-private.h"

SCHEME_EXPORT void init_scm_memory(scheme *sc);

SCHEME_EXPORT pointer scm_malloc(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_free(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_set_long(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_get_long(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_set_uchar(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_get_uchar(scheme *sc, pointer args);


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//初期化
void init_scm_memory(scheme *sc)
{
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-malloc"),sc->vptr->mk_foreign_func(sc, scm_malloc));
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-free"),sc->vptr->mk_foreign_func(sc, scm_free));

	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-set-long"),sc->vptr->mk_foreign_func(sc, scm_set_long));
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-get-long"),sc->vptr->mk_foreign_func(sc, scm_get_long));

	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-set-unsigned-char"),sc->vptr->mk_foreign_func(sc, scm_set_uchar));
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"mem-get-unsigned-char"),sc->vptr->mk_foreign_func(sc, scm_get_uchar));
}


pointer scm_malloc(scheme *sc, pointer args)
{
	pointer first_arg;
	pointer retval;

	void *ret;
	size_t sz;

	if ((args != sc->NIL) && sc->vptr->is_integer((first_arg = sc->vptr->pair_car(args))))
	{
		sz = (size_t)sc->vptr->ivalue(first_arg);
		ret = malloc(sz);
		if (NULL==ret)
		{
			retval = sc -> F;
		}
		else
		{
			retval = sc->vptr->mk_integer(sc, (long)ret);	//戻り値
		}
	}
	else
	{
		retval = sc -> F;
	}
	return retval;
}

pointer scm_free(scheme *sc, pointer args)
{
	pointer first_arg;
	pointer retval;

	void *mem;

	if ((args != sc->NIL) && sc->vptr->is_integer((first_arg = sc->vptr->pair_car(args))))
	{
		mem = (void *)sc->vptr->ivalue(first_arg);
		free(mem);
		retval = sc -> T;
	}
	else
	{
		retval = sc -> F;
	}
	return retval;
}

pointer scm_set_long(scheme *sc, pointer args)
{
	pointer arg_mem;
	pointer arg_loc;
	pointer arg_long;

	pointer retval;
	long *long_mem;
	long mem_loc;
	long val;

	arg_mem = sc->vptr->pair_car(args);
	arg_loc = sc->vptr->pair_car(sc->vptr->pair_cdr(args));
	arg_long = sc->vptr->pair_car(sc->vptr->pair_cdr(sc->vptr->pair_cdr(args)));

	long_mem = (long *)sc->vptr->ivalue(arg_mem);
	mem_loc = (long)sc->vptr->ivalue(arg_loc);
	val = (long)sc->vptr->ivalue(arg_long);

	long_mem[mem_loc] = val;
	retval = sc -> T;

	return retval;
}

pointer scm_get_long(scheme *sc, pointer args)
{
	pointer arg_mem;
	pointer arg_loc;
	pointer retval;

	long *long_mem;
	long mem_loc;
	long val;

	arg_mem = sc->vptr->pair_car(args);
	arg_loc = sc->vptr->pair_car(sc->vptr->pair_cdr(args));

	long_mem = (long *)sc->vptr->ivalue(arg_mem);
	mem_loc = (long)sc->vptr->ivalue(arg_loc);

	val = long_mem[mem_loc];
	retval = sc->vptr->mk_integer(sc ,val);

	return retval;
}


pointer scm_set_uchar(scheme *sc, pointer args)
{
	pointer arg_mem;
	pointer arg_loc;
	pointer arg_long;

	pointer retval;
	unsigned char *ucahr_mem;
	long mem_loc;
	long val;

	arg_mem = sc->vptr->pair_car(args);
	arg_loc = sc->vptr->pair_car(sc->vptr->pair_cdr(args));
	arg_long = sc->vptr->pair_car(sc->vptr->pair_cdr(sc->vptr->pair_cdr(args)));

	ucahr_mem = (long *)sc->vptr->ivalue(arg_mem);
	mem_loc = (long)sc->vptr->ivalue(arg_loc);
	val = (long)sc->vptr->ivalue(arg_long);

	ucahr_mem[mem_loc] = val;
	retval = sc -> T;

	return retval;
}

pointer scm_get_uchar(scheme *sc, pointer args)
{
	pointer arg_mem;
	pointer arg_loc;
	pointer retval;

	unsigned char *uchar_mem;
	long mem_loc;
	unsigned char val;

	arg_mem = sc->vptr->pair_car(args);
	arg_loc = sc->vptr->pair_car(sc->vptr->pair_cdr(args));

	uchar_mem = (unsigned char *)sc->vptr->ivalue(arg_mem);
	mem_loc = (long)sc->vptr->ivalue(arg_loc);

	val = uchar_mem[mem_loc];
	retval = sc->vptr->mk_integer(sc ,(long)val);

	return retval;
}

