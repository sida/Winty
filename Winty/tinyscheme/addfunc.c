#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>

#include "scheme.h"
#include "addfunc.h"
#include "scheme-private.h"
#include "..\wmcommandlog.h"

HWND hViewWnd;
HDC hViewDc;

//pointer scm_set_input_port_file(scheme *sc, pointer args)
//{
//	FILE *fin;
//	pointer first_arg;
//	pointer retval;
//
//	if ((args != sc->NIL) && is_port((first_arg = pair_car(args))))
//	{
//		fin = first_arg->_object._port->rep.stdio.file;
//		scheme_set_input_port_file(sc, fin);
//		retval = sc -> T;
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return retval;
//}

//pointer scm_set_output_port_file(scheme *sc, pointer args)
//{
//	FILE *fout;
//	pointer first_arg;
//	pointer retval;
//
//	if ((args != sc->NIL) && is_port((first_arg = pair_car(args))))
//	{
//		fout = first_arg->_object._port->rep.stdio.file;
//		scheme_set_output_port_file(sc, fout);
//		retval = sc -> T;
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return retval;
//}

//pointer scm_console_output_port(scheme *sc, pointer args)
//{
//	FILE *f;
//	HANDLE hStdout;
//	HANDLE hConsle;
//
//	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);	//�R���\�[���̕W���o��
//	hConsle = _open_osfhandle((long)hStdout, _O_TEXT);
//	f = _fdopen(hConsle, "w");
//	setvbuf(f, NULL, _IONBF, 0);
//
//	return port_from_file(sc, f ,port_output);
//}
//
//pointer scm_console_input_port(scheme *sc, pointer args)
//{
//	FILE *f;
//	HANDLE hStdin;
//	HANDLE hConsle;
//
//	hStdin = GetStdHandle(STD_INPUT_HANDLE);	//�R���\�[���̕W���o��
//	hConsle = _open_osfhandle((long)hStdin, _O_TEXT);
//	f = _fdopen(hConsle, "r");
//	setvbuf(f, NULL, _IONBF, 0);
//
//	return port_from_file(sc, f ,port_input);
//}


//pointer scm_malloc(scheme *sc, pointer args)
//{
//	pointer first_arg;
//	pointer retval;
//
//	void *ret;
//	size_t sz;
//
//	if ((args != sc->NIL) && is_integer((first_arg = pair_car(args))))
//	{
//		sz = (size_t)ivalue(first_arg);
//		ret = malloc(sz);
//		if (NULL==ret)
//		{
//			retval = sc -> F;
//		}
//		else
//		{
//			retval = mk_integer(sc, (long)ret);	//�߂�l
//		}
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return(retval);
//}
//
//pointer scm_free(scheme *sc, pointer args)
//{
//	pointer first_arg;
//	pointer retval;
//
//	void *mem;
//
//	if ((args != sc->NIL) && is_integer((first_arg = pair_car(args))))
//	{
//		mem = (void *)ivalue(first_arg);
//		free(mem);
//		retval = sc -> T;
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return(retval);
//}
//
//pointer scm_set_long(scheme *sc, pointer args)
//{
//	pointer arg_mem;
//	pointer arg_loc;
//	pointer arg_long;
//
//	pointer retval;
//	long *long_mem;
//	long mem_loc;
//	long val;
//
//	arg_mem = pair_car(args);
//	arg_loc = pair_car(pair_cdr(args));
//	arg_long = pair_car(pair_cdr(pair_cdr(args)));
//
//	long_mem = (long *)ivalue(arg_mem);
//	mem_loc = (long)ivalue(arg_loc);
//	val = (long)ivalue(arg_long);
//
//	long_mem[mem_loc] = val;
//	retval = sc -> T;
//
//	return(retval);
//}
//
//pointer scm_get_long(scheme *sc, pointer args)
//{
//	pointer arg_mem;
//	pointer arg_loc;
//	pointer retval;
//
//	long *long_mem;
//	long mem_loc;
//	long val;
//
//	arg_mem = pair_car(args);
//	arg_loc = pair_car(pair_cdr(args));
//
//	long_mem = (long *)ivalue(arg_mem);
//	mem_loc = (long)ivalue(arg_loc);
//
//	val = long_mem[mem_loc];
//	retval = mk_integer(sc ,val);
//
//	return(retval);
//}
//
//
//pointer scm_set_uchar(scheme *sc, pointer args)
//{
//	pointer arg_mem;
//	pointer arg_loc;
//	pointer arg_long;
//
//	pointer retval;
//	unsigned char *ucahr_mem;
//	long mem_loc;
//	long val;
//
//	arg_mem = pair_car(args);
//	arg_loc = pair_car(pair_cdr(args));
//	arg_long = pair_car(pair_cdr(pair_cdr(args)));
//
//	ucahr_mem = (long *)ivalue(arg_mem);
//	mem_loc = (long)ivalue(arg_loc);
//	val = (long)ivalue(arg_long);
//
//	ucahr_mem[mem_loc] = val;
//	retval = sc -> T;
//
//	return(retval);
//}
//
//pointer scm_get_uchar(scheme *sc, pointer args)
//{
//	pointer arg_mem;
//	pointer arg_loc;
//	pointer retval;
//
//	unsigned char *uchar_mem;
//	long mem_loc;
//	unsigned char val;
//
//	arg_mem = pair_car(args);
//	arg_loc = pair_car(pair_cdr(args));
//
//	uchar_mem = (unsigned char *)ivalue(arg_mem);
//	mem_loc = (long)ivalue(arg_loc);
//
//	val = uchar_mem[mem_loc];
//	retval = mk_integer(sc ,(long)val);
//
//	return(retval);
//}


//WM_COMMAND�o�b�t�@���P���b�Z�[�W���擾
pointer scm_getWmCommand(scheme *sc, pointer args)
{
	COMMANDLOG *plog;
	pointer phwnd;
	pointer plparam;
	pointer pwparam;
	pointer retval;

	plog = wmoutLog();
	if (NULL==plog) retval = sc -> F;
	else
	{
		//�߂�l�擾
		phwnd = mk_integer(sc ,plog->hWnd);
		plparam = mk_integer(sc ,plog->lParam);
		pwparam = mk_integer(sc ,plog->wParam);
		//�R�̖߂�l�����X�g�ɂ���
		retval = cons(sc, pwparam, sc->NIL);
		retval = cons(sc, plparam, retval);
		retval = cons(sc, phwnd, retval);
	}
	return(retval);
}

////�O���v���O�����̎��s
//pointer scm_system(scheme *sc, pointer args)
//{
//	pointer first_arg;
//	pointer retval;
//	char *command;
//	int ret;
//
//	if ((args != sc->NIL) && is_string((first_arg = pair_car(args))))
//	{
//		command = string_value(first_arg);
//		ret = system(command);
//		retval = mk_integer(sc, (long)ret);	//�߂�l
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return(retval);
//}

pointer scm_getViewhDc(scheme *sc, pointer args)
{
	pointer retval;
	if (NULL == hViewDc)
	{
		retval = sc -> F;
	}
	else
	{
		retval = mk_integer(sc, (long)hViewDc);	//View�̉��z�f�o�C�X�R���e�L�X�g��Ԃ�
	}
	return(retval);
}

pointer scm_getViewhWnd(scheme *sc, pointer args)
{
	pointer retval;
	if (NULL == hViewWnd)
	{
		retval = sc -> F;
	}
	else
	{
		retval = mk_integer(sc, (long)hViewWnd);	//View�̃E�C���h�E�n���h����Ԃ�
	}
	return(retval);
}

//pointer scm_LoadLibrary(scheme *sc, pointer args)
//{
//	pointer first_arg;
//	pointer retval;
//	char *name;
//	HMODULE dll_handle=NULL;
//
//	if ((args != sc->NIL) && is_string((first_arg = pair_car(args))))
//	{
//		name = string_value(first_arg);
//		dll_handle = LoadLibrary(name);
//		if (NULL == dll_handle)
//		{
//			retval = sc -> F;
//		}
//		else
//		{
//			retval = mk_integer(sc, (long)dll_handle);	//DLL�̃n���h����Ԃ�
//		}
//	}
//	else
//	{
//		retval = sc -> F;
//	}
//	return(retval);
//}
//
////TODO�F�����͌^�w�肵�ēn���΂������ȂƎv���Ă���Ȋ����iDLLARGS�j�ɂȂ��Ă邯��
////���܂̂Ƃ���|�C���^��������long�i�S�o�C�g�j�Ŏ��̂ł���Ȃ��Ȃ��B
//int listupArgs(scheme *sc ,DLLARGS *poutArg ,pointer args)
//{
//	int cnt;
//	pointer pcar_arg;
//	pointer tmpargs;
//	pointer pc;
//	long type;
//
//	cnt=0;
//	tmpargs = args;
//	while(tmpargs!=sc->NIL)
//	{
//		pcar_arg = pair_car(tmpargs);
//		tmpargs = pair_cdr(tmpargs);
//		if (1==is_integer(pcar_arg))
//		{//�^�w��Ȃ��̐��l�̈���
//			type=2;
//			poutArg[cnt].val = (void *)ivalue(pcar_arg);
//		}
//		else if (1==is_string(pcar_arg))
//		{//�^�w��Ȃ��̕�����̈���
//			type=1;
//			poutArg[cnt].val = (void *)string_value(pcar_arg);
//		}
//		else if (1==is_pair(pcar_arg))
//		{//�^�w��t���̈���
//			pc = pair_car(pcar_arg);
//			type = pc->_object._number.value.ivalue;
//			pc = pair_car(pair_cdr(pcar_arg));
//			switch(type)
//			{
//			case 1:	//void*
//				poutArg[cnt].val = (void *)string_value(pcar_arg);
//				break;
//			case 2:	//long
//				poutArg[cnt].val = (void *)ivalue(pcar_arg);
//				break;
//			case 3:	//HANDLE
//				poutArg[cnt].val = (void *)ivalue(pcar_arg);
//				break;
//			default:
//				poutArg[cnt].kind = 0;
//				return -1;
//			}
//		}
//		poutArg[cnt].kind = type;
//		cnt++;
//	}
//	poutArg[cnt].kind = 0;
//	return cnt;
//}
//
//pointer callDllFunc(scheme *sc ,HMODULE hDll ,const char *funcname ,DLLARGS *pArg)
//{
//	pointer retval;
//	int cnt=0;
//	long val;
//	long lret;
//
//	FARPROC procedure = GetProcAddress(hDll ,funcname);
//	if (NULL == procedure) return sc->F;
//
//	//__stdcall�͉E���獶�ɃX�^�b�N�ɐς�Ŗ߂�l��eax���W�X�^��
//	cnt=0;
//	while(0 != pArg[cnt].kind) cnt++;
//
//	while (0<cnt)
//	{
//		val = (long)pArg[cnt-1].val;
//		cnt--;
//		_asm
//		{
//			mov eax,val;
//			push eax;			//�����ݒ�
//		}
//	}
//	_asm
//	{
//		call [procedure];		//API�R�[��
//		mov lret,eax;
//	}
//	retval = mk_integer(sc, lret);	//API�̃��^�[���l�Ԃ�
//	return retval;
//}
//
////DLL�n���h���@�֐����@�������X�g�@
//pointer scm_call_Library(scheme *sc, pointer args)
//{
//	pointer dll_handle_arg;
//	pointer func_name_arg;
//	pointer c_arg_list;
//	pointer retval;
//
//	char *funcName;
//	HMODULE dll_handle;
//	static DLLARGS pDllargs[10];
//
//	dll_handle_arg = pair_car(args);
//	func_name_arg = pair_car(pair_cdr(args));
//	c_arg_list = pair_cdr(pair_cdr(args));
//
//	dll_handle = (HMODULE)ivalue(dll_handle_arg);	//DLL�n���h���擾
//	funcName = string_value(func_name_arg);	//DLL�֐����擾
//
//	//�Z�����������ăR�[����������𑵂���
//	listupArgs(sc,pDllargs,c_arg_list);
//
//	//viod * 1
//	//long 2
//	//HANDLE 3
//
//	retval = callDllFunc(sc ,dll_handle ,funcName ,pDllargs);
//	return(retval);
//}
//
//
