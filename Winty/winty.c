// winty.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "winty.h"
#include "workerthrd.h"
#include "wmcommandlog.h"

#define MAX_LOADSTRING 100

// グローバル変数 :
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

extern HANDLE m_hThread;	//スレッドのハンドル
extern HWND hViewWnd;
extern HDC hViewDc;

HBITMAP m_hBmp;
BOOL m_consoleOn;
HWND m_consoleWindow;

// このコード モジュールに含まれる関数の宣言を転送します :
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINTY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	m_hThread = NULL;
	m_consoleOn = FALSE;
	m_consoleWindow = NULL;
	hViewWnd = NULL;
	hViewDc = NULL;
	m_hBmp = NULL;

	// アプリケーションの初期化を実行します。:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINTY);

	// メイン メッセージ ループ :
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
//  関数 : MyRegisterClass()
//
//  目的 : ウィンドウ クラスを登録します。
//
//  コメント :
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//     Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた 
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
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
//   関数 : InitInstance(HANDLE, int)
//
//   目的 : インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント :
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   hViewWnd = hWnd;	//ウインドウハンドルの保存。schemeとの連携用

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数 : WndProc(HWND, unsigned, WORD, LONG)
//
//  目的 :  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
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

		hdc = GetDC(hWnd);					//画面のデバイスコンテキストを取得する
		hViewDc = CreateCompatibleDC(hdc);		//画面と同じデバイスコンテキストをメモリ上に作成する
		ReleaseDC(hWnd ,hdc);

		hPen = (HPEN)GetStockObject( WHITE_PEN );	// 白いストックペンのハンドルを取得する
		SelectObject( hViewDc, hPen );					// 取得したペンを使用する

		create_schme_thread();
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// 選択されたメニューの解析 :
		wminLog(hWnd ,wParam ,lParam);	//メニュースキャン用にFIFOバッファに保存
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
					CheckMenuItem(hMenu,wmId,MF_BYCOMMAND|MF_CHECKED);	//チェックつける
					ShowWindow(m_consoleWindow,SW_SHOW);				//コンソールの表示
				}
				else
				{
					CheckMenuItem(hMenu,wmId,MF_BYCOMMAND|MF_UNCHECKED);	//チェック消す
					ShowWindow(m_consoleWindow,SW_HIDE);				//コンソールの非表示
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
		// TODO: 描画コードをここに追加してください...
		/*  描画を行う  （ここから）    */
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
			//新画面
			hdc = GetDC(hWnd);								//画面のデバイスコンテキストを取得する
			m_hBmp = CreateCompatibleBitmap(hdc, cx,cy);	//BitMap作成
			SelectObject(hViewDc, m_hBmp);					// 裏画面を作成したビットマップに関連付ける。
			if (NULL!=hOldBmp)
			{
				//旧画面
				tmphdc = CreateCompatibleDC(hdc);		//画面と同じデバイスコンテキストをメモリ上に作成する
				SelectObject(tmphdc, hOldBmp);
				//新＜－－旧コピー
				BitBlt(hViewDc ,0 ,0 ,cx ,cy ,tmphdc ,0 ,0 ,SRCCOPY);
				DeleteDC(tmphdc);
				DeleteObject(hOldBmp);
			}
			ReleaseDC(hWnd ,hdc);
		}
		break;
	case WM_DESTROY:
		CloseHandle(m_hThread);	//スレッドのクローズ
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
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
