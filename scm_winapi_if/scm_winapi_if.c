// scm_winapi_if.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#define _SCHEME_SOURCE
#include "..\Winty\tinyscheme\scheme-private.h"

SCHEME_EXPORT void init_scm_winapi_if(scheme *sc);
SCHEME_EXPORT pointer scm_LoadLibrary(scheme *sc, pointer args);
SCHEME_EXPORT pointer scm_call_Library(scheme *sc, pointer args);

typedef struct _dllargs
{
	int kind;
	void *val;
} DLLARGS;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//������
void init_scm_winapi_if(scheme *sc)
{
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"load-library"),sc->vptr->mk_foreign_func(sc, scm_LoadLibrary));
	sc->vptr->scheme_define(sc,sc->global_env,sc->vptr->mk_symbol(sc,"call-library"),sc->vptr->mk_foreign_func(sc, scm_call_Library));  
}


pointer scm_LoadLibrary(scheme *sc, pointer args)
{
	pointer first_arg;
	pointer retval;
	char *name;
	HMODULE dll_handle=NULL;

	if ((args != sc->NIL) && sc->vptr->is_string((first_arg = sc->vptr->pair_car(args))))
	{
		name = sc->vptr->string_value(first_arg);
		dll_handle = LoadLibrary(name);
		if (NULL == dll_handle)
		{
			retval = sc -> F;
		}
		else
		{
			retval = sc->vptr->mk_integer(sc, (long)dll_handle);	//DLL�̃n���h����Ԃ�
		}
	}
	else
	{
		retval = sc -> F;
	}
	return(retval);
}

//TODO�F�����͌^�w�肵�ēn���΂������ȂƎv���Ă���Ȋ����iDLLARGS�j�ɂȂ��Ă邯��
//���܂̂Ƃ���|�C���^��������long�i�S�o�C�g�j�Ŏ��̂ł���Ȃ��Ȃ��B
int listupArgs(scheme *sc ,DLLARGS *poutArg ,pointer args)
{
	int cnt;
	pointer pcar_arg;
	pointer tmpargs;
	pointer pc;
	long type;

	cnt=0;
	tmpargs = args;
	while(tmpargs!=sc->NIL)
	{
		pcar_arg = sc->vptr->pair_car(tmpargs);
		tmpargs = sc->vptr->pair_cdr(tmpargs);
		if (1==sc->vptr->is_integer(pcar_arg))
		{//�^�w��Ȃ��̐��l�̈���
			type=2;
			poutArg[cnt].val = (void *)sc->vptr->ivalue(pcar_arg);
		}
		else if (1==sc->vptr->is_string(pcar_arg))
		{//�^�w��Ȃ��̕�����̈���
			type=1;
			poutArg[cnt].val = (void *)sc->vptr->string_value(pcar_arg);
		}
		else if (1==sc->vptr->is_pair(pcar_arg))
		{//�^�w��t���̈���
			pc = sc->vptr->pair_car(pcar_arg);
			//type = pc->_object._number.value.ivalue;
			type = sc->vptr->ivalue(pc);
			pc = sc->vptr->pair_car(sc->vptr->pair_cdr(pcar_arg));
			switch(type)
			{
			case 1:	//void*
				poutArg[cnt].val = (void *)sc->vptr->string_value(pcar_arg);
				break;
			case 2:	//long
				poutArg[cnt].val = (void *)sc->vptr->ivalue(pcar_arg);
				break;
			case 3:	//HANDLE
				poutArg[cnt].val = (void *)sc->vptr->ivalue(pcar_arg);
				break;
			default:
				poutArg[cnt].kind = 0;
				return -1;
			}
		}
		poutArg[cnt].kind = type;
		cnt++;
	}
	poutArg[cnt].kind = 0;
	return cnt;
}

pointer callDllFunc(scheme *sc ,HMODULE hDll ,const char *funcname ,DLLARGS *pArg)
{
	pointer retval;
	int cnt=0;
	long val;
	long lret;

	FARPROC procedure = GetProcAddress(hDll ,funcname);
	if (NULL == procedure) return sc->F;

	//__stdcall�͉E���獶�ɃX�^�b�N�ɐς�Ŗ߂�l��eax���W�X�^��
	cnt=0;
	while(0 != pArg[cnt].kind) cnt++;

	while (0<cnt)
	{
		val = (long)pArg[cnt-1].val;
		cnt--;
		_asm
		{
			mov eax,val;
			push eax;			//�����ݒ�
		}
	}
	_asm
	{
		call [procedure];		//API�R�[��
		mov lret,eax;
	}
	retval = sc->vptr->mk_integer(sc, lret);	//API�̃��^�[���l�Ԃ�
	return retval;
}

//DLL�n���h���@�֐����@�������X�g�@
pointer scm_call_Library(scheme *sc, pointer args)
{
	pointer dll_handle_arg;
	pointer func_name_arg;
	pointer c_arg_list;
	pointer retval;

	char *funcName;
	HMODULE dll_handle;
	static DLLARGS pDllargs[10];

	dll_handle_arg = sc->vptr->pair_car(args);
	func_name_arg = sc->vptr->pair_car(sc->vptr->pair_cdr(args));
	c_arg_list = sc->vptr->pair_cdr(sc->vptr->pair_cdr(args));

	dll_handle = (HMODULE)sc->vptr->ivalue(dll_handle_arg);	//DLL�n���h���擾
	funcName = sc->vptr->string_value(func_name_arg);	//DLL�֐����擾

	//�Z�����������ăR�[����������𑵂���
	listupArgs(sc,pDllargs,c_arg_list);

	//viod * 1
	//long 2
	//HANDLE 3

	retval = callDllFunc(sc ,dll_handle ,funcName ,pDllargs);
	return(retval);
}


