#ifndef __WMCOMMANDLOG_H
#define __WMCOMMANDLOG_H

typedef struct _commandlog
{
	HWND hWnd;
	WPARAM wParam;
	LPARAM lParam;
} COMMANDLOG;

BOOL wminLog(HWND hWnd, WPARAM wParam, LPARAM lParam);
COMMANDLOG *wmoutLog(void);
void lockLogAccsee(BOOL l);
BOOL getlockLogAccsee(void);

#endif
