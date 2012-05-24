// winty.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "winty.h"
#include "workerthrd.h"
#include "wmcommandlog.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ� :
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

extern HANDLE m_hThread;	//�X���b�h�̃n���h��
extern HWND hViewWnd;
extern HDC hViewDc;

HBITMAP m_hBmp;
BOOL m_consoleOn;
HWND m_consoleWindow;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂� :
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINTY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	m_hThread = NULL;
	m_consoleOn = FALSE;
	m_consoleWindow = NULL;
	hViewWnd = NULL;
	hViewDc = NULL;
	m_hBmp = NULL;

	// �A�v���P�[�V�����̏����������s���܂��B:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINTY);

	// ���C�� ���b�Z�[�W ���[�v :
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �֐� : MyRegisterClass()
//
//  �ړI : �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g :
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//     Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ 
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WINTY);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_WINTY;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   �֐� : InitInstance(HANDLE, int)
//
//   �ړI : �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g :
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   hViewWnd = hWnd;	//�E�C���h�E�n���h���̕ۑ��Bscheme�Ƃ̘A�g�p

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �֐� : WndProc(HWND, unsigned, WORD, LONG)
//
//  �ړI :  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT Rect;
	HPEN hPen;

	HBITMAP hOldBmp;
	HDC tmphdc;

	switch (message) 
	{
	case WM_CREATE:
		GetClientRect(hWnd ,&Rect);

		hdc = GetDC(hWnd);					//��ʂ̃f�o�C�X�R���e�L�X�g���擾����
		hViewDc = CreateCompatibleDC(hdc);		//��ʂƓ����f�o�C�X�R���e�L�X�g����������ɍ쐬����
		ReleaseDC(hWnd ,hdc);

		hPen = (HPEN)GetStockObject( WHITE_PEN );	// �����X�g�b�N�y���̃n���h�����擾����
		SelectObject( hViewDc, hPen );					// �擾�����y�����g�p����

		create_schme_thread();
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// �I�����ꂽ���j���[�̉�� :
		wminLog(hWnd ,wParam ,lParam);	//���j���[�X�L�����p��FIFO�o�b�t�@�ɕۑ�
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_CONSOLE:
			{
				HMENU hMenu;
				hMenu = GetMenu(hWnd);
				if (!hMenu) break;
				m_consoleOn = !m_consoleOn;
				if (m_consoleOn)
				{
					CheckMenuItem(hMenu,wmId,MF_BYCOMMAND|MF_CHECKED);	//�`�F�b�N����
					ShowWindow(m_consoleWindow,SW_SHOW);				//�R���\�[���̕\��
				}
				else
				{
					CheckMenuItem(hMenu,wmId,MF_BYCOMMAND|MF_UNCHECKED);	//�`�F�b�N����
					ShowWindow(m_consoleWindow,SW_HIDE);				//�R���\�[���̔�\��
				}
			}
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		/*  �`����s��  �i��������j    */
		GetClientRect(hWnd ,&Rect);
		BitBlt(hdc ,0 ,0 ,Rect.right ,Rect.bottom ,hViewDc ,0 ,0 ,SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		if (NULL!=hViewWnd)
		{
			int cx,cy;
			hOldBmp = m_hBmp;
			cx = LOWORD(lParam); 
			cy = HIWORD(lParam); 
			//�V���
			hdc = GetDC(hWnd);								//��ʂ̃f�o�C�X�R���e�L�X�g���擾����
			m_hBmp = CreateCompatibleBitmap(hdc, cx,cy);	//BitMap�쐬
			SelectObject(hViewDc, m_hBmp);					// ����ʂ��쐬�����r�b�g�}�b�v�Ɋ֘A�t����B
			if (NULL!=hOldBmp)
			{
				//�����
				tmphdc = CreateCompatibleDC(hdc);		//��ʂƓ����f�o�C�X�R���e�L�X�g����������ɍ쐬����
				SelectObject(tmphdc, hOldBmp);
				//�V���|�|���R�s�[
				BitBlt(hViewDc ,0 ,0 ,cx ,cy ,tmphdc ,0 ,0 ,SRCCOPY);
				DeleteDC(tmphdc);
				DeleteObject(hOldBmp);
			}
			ReleaseDC(hWnd ,hdc);
		}
		break;
	case WM_DESTROY:
		CloseHandle(m_hThread);	//�X���b�h�̃N���[�Y
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
