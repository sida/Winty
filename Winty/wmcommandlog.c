//WM_COMMANDのFIFOバッファ
//バッファがあふれたら古い順に破棄する

#include "windows.h"
#include "wmcommandlog.h"

#define MAXBUFFER 100
static long point_bffer=0;
static long cnt=0;
static BOOL lockflg=FALSE;
static COMMANDLOG pLogBuffer[MAXBUFFER];

//ログ追加
//ウインドウから追加される
BOOL wminLog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	while (getlockLogAccsee());	//ロック解除まで待機
	lockLogAccsee(TRUE);		//ロック
	pLogBuffer[point_bffer].hWnd = hWnd;
	pLogBuffer[point_bffer].lParam = lParam;
	pLogBuffer[point_bffer].wParam = wParam;
	point_bffer++;
	if (MAXBUFFER<=point_bffer) point_bffer=0;
	cnt++;
	if (MAXBUFFER<=cnt) cnt=MAXBUFFER;
	lockLogAccsee(FALSE);		//アンロック
	return TRUE;
}

//ログ取得
//Schemeからアクセスされる
COMMANDLOG *wmoutLog(void)
{
	if (0==cnt) return NULL;
	while (getlockLogAccsee());	//ロック解除まで待機
	lockLogAccsee(TRUE);		//ロック
	if (0 >= point_bffer) point_bffer = MAXBUFFER;
	point_bffer--;
	cnt--;
	lockLogAccsee(FALSE);		//アンロック
	return &pLogBuffer[point_bffer];
}

//ロックの設定＆解除
void lockLogAccsee(BOOL l)
{
	lockflg = l;
}

//ロック状態取得
BOOL getlockLogAccsee(void)
{
	return lockflg;
}

