
#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "Windows.h"
#include "workerthrd.h"

#include "tinyscheme\scheme.h"

HANDLE m_hThread;	//�X���b�h�̃n���h��
extern int main(int argc, char **argv);
extern HWND m_consoleWindow;
extern HWND hViewWnd;

DWORD WINAPI WorkerThreadProc(LPVOID pParam)
{
	char *argv[] = {"tinyscheme","",""};
	main(1,argv);	//tiny scheme��main
	FreeConsole();
	PostQuitMessage(0);	//�A�v�����I������͂������E�E�E�Ȃ������Ȃ�
	return 0;
}


void create_schme_thread(void)
{
	char consolename[1000]; 
	int hConsle;
	HANDLE hStdin;
	HANDLE hStdout;
	HANDLE hStderr;
	DWORD dwThreadId;

	*consolename = '\0';
	AllocConsole();										//�R���\�[������
	////Sleep(3000);
	GetConsoleTitle(consolename,sizeof(consolename));	//�R���\�[���̃^�C�g���擾
	m_consoleWindow = FindWindow(NULL,consolename);		//�^�C�g������n���h�����擾
	ShowWindow(m_consoleWindow,SW_HIDE);				//�R���\�[���̔�\��

	hStdin = GetStdHandle(STD_INPUT_HANDLE);	//�R���\�[���̕W������
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);	//�R���\�[���̕W���o��
	hStderr = GetStdHandle(STD_ERROR_HANDLE);	//�R���\�[���̕W���G���[

	hConsle = _open_osfhandle((long)hStdout, _O_TEXT); 
	*stdout = *_fdopen(hConsle, "w");
	setvbuf(stdout, NULL, _IONBF, 0);

	hConsle = _open_osfhandle((long)hStdin, _O_TEXT);
	*stdin = *_fdopen(hConsle, "r");
	setvbuf(stdin, NULL, _IONBF, 0);

	m_hThread = CreateThread(
		NULL,		//�f�t�H���g�̃Z�L�����e�B�[
		0,			//�X�^�b�N�T�C�Y�̓f�t�H���g��
		WorkerThreadProc,
		0,			//LPVOID lpParameter�@���Ƀp�����[�^�͓n���Ȃ�
		0,			//��������Ɏ��s
		&dwThreadId	//�X���b�hID���Ԃ�B���Ɏg��Ȃ�
	);
}
