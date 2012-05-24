
#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "Windows.h"
#include "workerthrd.h"

#include "tinyscheme\scheme.h"

HANDLE m_hThread;	//スレッドのハンドル
extern int main(int argc, char **argv);
extern HWND m_consoleWindow;
extern HWND hViewWnd;

DWORD WINAPI WorkerThreadProc(LPVOID pParam)
{
	char *argv[] = {"tinyscheme","",""};
	main(1,argv);	//tiny schemeのmain
	FreeConsole();
	PostQuitMessage(0);	//アプリが終了するはずだが・・・なぜかしない
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
	AllocConsole();										//コンソール生成
	////Sleep(3000);
	GetConsoleTitle(consolename,sizeof(consolename));	//コンソールのタイトル取得
	m_consoleWindow = FindWindow(NULL,consolename);		//タイトルからハンドルを取得
	ShowWindow(m_consoleWindow,SW_HIDE);				//コンソールの非表示

	hStdin = GetStdHandle(STD_INPUT_HANDLE);	//コンソールの標準入力
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);	//コンソールの標準出力
	hStderr = GetStdHandle(STD_ERROR_HANDLE);	//コンソールの標準エラー

	hConsle = _open_osfhandle((long)hStdout, _O_TEXT); 
	*stdout = *_fdopen(hConsle, "w");
	setvbuf(stdout, NULL, _IONBF, 0);

	hConsle = _open_osfhandle((long)hStdin, _O_TEXT);
	*stdin = *_fdopen(hConsle, "r");
	setvbuf(stdin, NULL, _IONBF, 0);

	m_hThread = CreateThread(
		NULL,		//デフォルトのセキュリティー
		0,			//スタックサイズはデフォルトで
		WorkerThreadProc,
		0,			//LPVOID lpParameter　特にパラメータは渡さない
		0,			//生成直後に実行
		&dwThreadId	//スレッドIDが返る。特に使わない
	);
}
