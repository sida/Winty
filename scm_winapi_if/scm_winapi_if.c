// scm_winapi_if.cpp : DLL アプリケーションのエントリ ポイントを定義します。
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

//初期化
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
			retval = sc->vptr->mk_integer(sc, (long)dll_handle);	//DLLのハンドラを返す
		}
	}
	else
	{
		retval = sc -> F;
	}
	return(retval);
}

//TODO：引数は型指定して渡せばいいかなと思ってこんな感じ（DLLARGS）になってるけど
//いまのところポインタも整数もlong（４バイト）で持つのでいらないなぁ。
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
		{//型指定なしの数値の引数
			type=2;
			poutArg[cnt].val = (void *)sc->vptr->ivalue(pcar_arg);
		}
		else if (1==sc->vptr->is_string(pcar_arg))
		{//型指定なしの文字列の引数
			type=1;
			poutArg[cnt].val = (void *)sc->vptr->string_value(pcar_arg);
		}
		else if (1==sc->vptr->is_pair(pcar_arg))
		{//型指定付きの引数
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

	//__stdcallは右から左にスタックに積んで戻り値はeaxレジスタに
	cnt=0;
	while(0 != pArg[cnt].kind) cnt++;

	while (0<cnt)
	{
		val = (long)pArg[cnt-1].val;
		cnt--;
		_asm
		{
			mov eax,val;
			push eax;			//引数設定
		}
	}
	_asm
	{
		call [procedure];		//APIコール
		mov lret,eax;
	}
	retval = sc->vptr->mk_integer(sc, lret);	//APIのリターン値返す
	return retval;
}

//DLLハンドル　関数名　引数リスト　
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

	dll_handle = (HMODULE)sc->vptr->ivalue(dll_handle_arg);	//DLLハンドル取得
	funcName = sc->vptr->string_value(func_name_arg);	//DLL関数名取得

	//セルをたぐってコールする引数を揃える
	listupArgs(sc,pDllargs,c_arg_list);

	//viod * 1
	//long 2
	//HANDLE 3

	retval = callDllFunc(sc ,dll_handle ,funcName ,pDllargs);
	return(retval);
}


