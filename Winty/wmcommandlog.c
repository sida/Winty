//WM_COMMAND��FIFO�o�b�t�@
//�o�b�t�@�����ӂꂽ��Â����ɔj������

#include "windows.h"
#include "wmcommandlog.h"

#define MAXBUFFER 100
static long point_bffer=0;
static long cnt=0;
static BOOL lockflg=FALSE;
static COMMANDLOG pLogBuffer[MAXBUFFER];

//���O�ǉ�
//�E�C���h�E����ǉ������
BOOL wminLog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	while (getlockLogAccsee());	//���b�N�����܂őҋ@
	lockLogAccsee(TRUE);		//���b�N
	pLogBuffer[point_bffer].hWnd = hWnd;
	pLogBuffer[point_bffer].lParam = lParam;
	pLogBuffer[point_bffer].wParam = wParam;
	point_bffer++;
	if (MAXBUFFER<=point_bffer) point_bffer=0;
	cnt++;
	if (MAXBUFFER<=cnt) cnt=MAXBUFFER;
	lockLogAccsee(FALSE);		//�A�����b�N
	return TRUE;
}

//���O�擾
//Scheme����A�N�Z�X�����
COMMANDLOG *wmoutLog(void)
{
	if (0==cnt) return NULL;
	while (getlockLogAccsee());	//���b�N�����܂őҋ@
	lockLogAccsee(TRUE);		//���b�N
	if (0 >= point_bffer) point_bffer = MAXBUFFER;
	point_bffer--;
	cnt--;
	lockLogAccsee(FALSE);		//�A�����b�N
	return &pLogBuffer[point_bffer];
}

//���b�N�̐ݒ聕����
void lockLogAccsee(BOOL l)
{
	lockflg = l;
}

//���b�N��Ԏ擾
BOOL getlockLogAccsee(void)
{
	return lockflg;
}

