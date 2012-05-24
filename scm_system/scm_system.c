// scm_system.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#define _SCHEME_SOURCE
#include "..\Winty\tinyscheme\scheme-private.h"

SCHEME_EXPORT void init_scm_system(scheme *sc);
SCHEME_EXPORT pointer scm_system(scheme *sc, pointer args);


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//������
void init_scm_system(scheme *sc)
{
	sc->vptr->scheme_define(
		sc
		,sc->global_env
		,sc->vptr->mk_symbol(sc,"system")
		,sc->vptr->mk_foreign_func(sc, scm_system)
	);
}

//�O���v���O�����̎��s
pointer scm_system(scheme *sc, pointer args)
{
	pointer first_arg;
	pointer retval;
	char *command;
	int ret;

	if ((args != sc->NIL) && sc->vptr->is_string((first_arg = sc->vptr->pair_car(args))))
	{
		command = sc->vptr->string_value(first_arg);
		ret = system(command);
		retval = sc->vptr->mk_integer(sc, (long)ret);	//�߂�l
	}
	else
	{
		retval = sc -> F;
	}
	return(retval);
}
