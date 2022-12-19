#include "clock.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("clock");
	WNDCLASS wndclass;
	MSG msg;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wndclass);
	hWnd_clock = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_LAYERED, szAppName, TEXT("Alarm clock"), WS_POPUP, 1200, 20, 200, 50, NULL, NULL, hInstance, NULL);

	SetWindowPos(hWnd_clock, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);
	ShowWindow(hWnd_clock, iCmdShow);
	UpdateWindow(hWnd_clock);
	SetLayeredWindowAttributes(hWnd_clock, RGB(255, 255, 255), 255, LWA_COLORKEY);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // 翻译虚拟键代码为字符
		DispatchMessage(&msg); //分发消息
	}
	return msg.wParam; //返回PostQuitMessage 函数中给定的退出代码。
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;	
	static HMENU hMenu;
	static HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	switch (msg)
	{
		case WM_CREATE:   
		{
			InitTray(hInstance, hWnd);
			RegisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW, MOD_ALT | MOD_CONTROL,'F');
			RegisterHotKey(hWnd, ID_HOTKEY_CLOSEALARM, MOD_ALT | MOD_CONTROL, 'C');
			
			RegisterHotKey(hWnd, ID_HOTKEY_LEFT, MOD_ALT | MOD_CONTROL, VK_NUMPAD4);
			RegisterHotKey(hWnd, ID_HOTKEY_RIGHT, MOD_ALT | MOD_CONTROL, VK_NUMPAD6);
			RegisterHotKey(hWnd, ID_HOTKEY_UP, MOD_ALT | MOD_CONTROL, VK_NUMPAD8);
			RegisterHotKey(hWnd, ID_HOTKEY_DOWM ,MOD_ALT | MOD_CONTROL, VK_NUMPAD2);

			hWnd_desktop = FindWindow(NULL, TEXT("FolderView"));
			SetParent(hWnd_clock, hWnd_desktop);
		
			hWnd_time = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, 260, 129, hWnd, NULL, hInstance, NULL);
			hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
			hMenu = GetSubMenu(hMenu, 0);
			hFont = CreateFont(-30, -15, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, TEXT("微软雅黑"));
			SetWindowLong(hWnd_clock, GWL_STYLE, WS_CHILD);
			SetTimer(hWnd, 0, 900, NULL);  //当回调函数填NULL时，是将消息发送到默认回调函数，详见下方WM_TIMER
			SendMessage(hWnd_time, WM_SETFONT, (WPARAM)hFont, NULL);
			CheckMenuItem(hMenu, ID_MENU_40001, MF_CHECKED);
			return 0;
		}
		case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case BN_CLICKED: 
				{
					switch (LOWORD(wParam))
					{
						case ID_MENU_40001:   //显示
						{
							ShowWindow(hWnd_clock, SW_SHOW);
							CheckMenuItem(hMenu, ID_MENU_40001, MF_CHECKED);
							CheckMenuItem(hMenu, ID_MENU_40002, MF_UNCHECKED);

							return 0;
						}
						case ID_MENU_40002:   //隐藏
						{
							ShowWindow(hWnd_clock, SW_HIDE);
							CheckMenuItem(hMenu, ID_MENU_40002, MF_CHECKED);
							CheckMenuItem(hMenu, ID_MENU_40001, MF_UNCHECKED);

							return 0;
						}
						case ID_MENU_40003:  //设置
						{
							DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd_clock, SettingProc);
							return 0;
						}
						case ID_MENU_40006:  //关闭响铃
						{
							//
							NoBackGround(L"C:\\Users\\gkhcsc\\Desktop\\08.bmp", L"C:\\Users\\gkhcsc\\Desktop\\08M.bmp");
							//
							_TerminateProcess();
							return 0;
						}
						case ID_MENU_40004: //退出
						{
							UnregisterHotKey(hWnd, ID_HOTKEY_CLOSEALARM);
							UnregisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW);
							_TerminateProcess();
							KillTimer(0, ID_ALARM);
							UnregisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW);
							Shell_NotifyIcon(NIM_DELETE, &nid);
							PostQuitMessage(0);
							return 0;
						}
						case ID_MENU_40005:  //闹铃
						{
							DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd_clock, AlarmProc);
							return 0;
						}
					}
				}
			}
			return 0;
		}
		case WM_LBUTTONDOWN: 
		{
			SendMessage(hWnd_clock, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
			return 0;
		}
		case WM_TRAY:
		{
			POINT pt;
			if (lParam == WM_RBUTTONDOWN)//托盘消息中lParam接受鼠标的行为
			{
				GetCursorPos(&pt);
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
				//SetForegroundWindow();
		
			}
			return 0;
		}
		case WM_CTLCOLORSTATIC:
		{
			if ((HWND)lParam == hWnd_time) 
			{
				HBRUSH hBursh = (HBRUSH)GetStockObject(WHITE_BRUSH);
				SetTextColor((HDC)wParam, RGB(RGB_R, RGB_G, RGB_B));
				SetBkColor((HDC)wParam, RGB(RGB_R, RGB_G, RGB_B));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (INT_PTR)hBursh;
			}
		}
		case WM_HOTKEY: 
		{
			switch (wParam)
			{
				case ID_HOTKEY_HIDEORSHOW: 
				{
					if (IsWindowVisible(hWnd_clock))  //如果窗口显示，则隐藏 
					{
						ShowWindow(hWnd_clock, SW_HIDE);
						CheckMenuItem(hMenu, ID_MENU_40002, MF_CHECKED);
						CheckMenuItem(hMenu, ID_MENU_40001, MF_UNCHECKED);
					}
					else
					{
						ShowWindow(hWnd_clock, SW_SHOW);
						CheckMenuItem(hMenu, ID_MENU_40001, MF_CHECKED);
						CheckMenuItem(hMenu, ID_MENU_40002, MF_UNCHECKED);
					}
					return 0;
				}
				case ID_HOTKEY_CLOSEALARM: 
				{
					_TerminateProcess();
					return 0;
				}
				case ID_HOTKEY_LEFT: 
				{
					RECT rc;
					GetWindowRect(hWnd_clock, &rc);
					MoveWindow(hWnd_clock, rc.left - MovePrePixels, rc.top,rc.right-rc.left, rc.bottom-rc.top, TRUE);
					return 0;
				}
				case ID_HOTKEY_RIGHT: 
				{
					RECT rc;
					GetWindowRect(hWnd_clock, &rc);
					MoveWindow(hWnd_clock, rc.left + MovePrePixels, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
					return 0;
				}
				case ID_HOTKEY_UP:
				{
					RECT rc;
					GetWindowRect(hWnd_clock, &rc);
					MoveWindow(hWnd_clock, rc.left, rc.top - MovePrePixels, rc.right - rc.left, rc.bottom - rc.top, TRUE);
					return 0;
				}
				case ID_HOTKEY_DOWM: 
				{
					RECT rc;
					GetWindowRect(hWnd_clock, &rc);
					MoveWindow(hWnd_clock, rc.left, rc.top + MovePrePixels, rc.right - rc.left, rc.bottom - rc.top, TRUE);
					return 0;
				}
			}
			return 0;
		}
		case WM_TIMER:
		{
			struct tm* CurrentTime;
			time_t t = time(NULL);
			CurrentTime = localtime(&t);
			TCHAR szBuffer[50];
			wsprintf(szBuffer, TEXT("%d时%d分%d秒"), CurrentTime->tm_hour, CurrentTime->tm_min, CurrentTime->tm_sec);
			SetWindowText(hWnd_time, szBuffer);
			HDC hdc = GetDC(NULL);
			HDC memDC = CreateCompatibleDC(hdc);
			BLENDFUNCTION _Blend;
			_Blend.BlendOp = 0;
			_Blend.BlendFlags = 0;
			_Blend.AlphaFormat = 1;
			_Blend.SourceConstantAlpha = 255;
			HBITMAP bmp;
			bmp = ::CreateCompatibleBitmap(hdc, 600, 74);
			::SelectObject(memDC, bmp);
			POINT p2 = { 0,0 };
			UpdateLayeredWindow(hWnd_clock, hdc, NULL, NULL, memDC, &p2, NULL, &_Blend, ULW_ALPHA);
			return 0;

		}
		case WM_DESTROY:
		{
			UnregisterHotKey(hWnd, ID_HOTKEY_CLOSEALARM);
			UnregisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW);
			_TerminateProcess();
			KillTimer(0, ID_ALARM);
			UnregisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW);
			Shell_NotifyIcon(NIM_DELETE, &nid);
			PostQuitMessage(0);
			return 0;
		}

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void InitTray(HINSTANCE hInstance, HWND hWnd)
{
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_MESSAGE | NIF_TIP|NIF_ICON;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcscpy(nid.szTip, L"时钟");
	Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL CALLBACK SettingProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	unsigned int a = 0;
	switch (msg)
	{
		case WM_INITDIALOG: 
		{
			if (isHaveValue() == TRUE)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_AutoRun), BM_SETCHECK, 1, 0);
			}
			DWORD flag;
			GetLayeredWindowAttributes(hWnd_clock, NULL, NULL, &flag);
			if (flag == LWA_ALPHA) 
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK_BackGround), BM_SETCHECK, 1, 0);
			}
			return TRUE;
		}
		case WM_COMMAND: 
		{
			a = HIWORD(wParam);
			switch (HIWORD(wParam))
			{
				
				case EN_CHANGE: 
				{
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_EDIT1))
					{	
						WCHAR GetMovePrePixels[4] ;
						GetWindowText(GetDlgItem(hWnd, IDC_EDIT1), GetMovePrePixels, 4);
						MovePrePixels = _wtoi(GetMovePrePixels);
						return 0;
					}
						
					TCHAR cRGB_R[4];
					TCHAR cRGB_G[4];
					TCHAR cRGB_B[4];
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), cRGB_R, 4);
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT3), cRGB_G, 4);
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT5), cRGB_B, 4);
					RGB_R2 = _wtoi(cRGB_R);
					RGB_G2 = _wtoi(cRGB_G);
					RGB_B2 = _wtoi(cRGB_B);
					SetWindowText(GetDlgItem(hWnd, IDC_STATIC4), NULL);
					return TRUE;
				}
				case BN_CLICKED: 
				{
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_BUTTON1)) 
					{
						TCHAR cRGB_R[4];
						TCHAR cRGB_G[4];
						TCHAR cRGB_B[4];
						GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), cRGB_R, 4);
						GetWindowText(GetDlgItem(hWnd, IDC_EDIT3), cRGB_G, 4);
						GetWindowText(GetDlgItem(hWnd, IDC_EDIT5), cRGB_B, 4);
						RGB_R = _wtoi(cRGB_R);
						RGB_G = _wtoi(cRGB_G);
						RGB_B = _wtoi(cRGB_B);
						return TRUE;
					}
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_CHECK_AutoRun)) 
					{
						if (IsDlgButtonChecked(hWnd, IDC_CHECK_AutoRun) == BST_UNCHECKED)
						{
							if (isHaveValue() == FALSE)
							{
								SendMessage((HWND)lParam, BM_SETCHECK, 1, 0);
								SetAutoRun();
								return TRUE;
							}
							return FALSE;
						}
							

						if (IsDlgButtonChecked(hWnd, IDC_CHECK_AutoRun) == BST_CHECKED) 
						{
							SendMessage((HWND)lParam, BM_SETCHECK, 0, 0);
							DeleteAutoRun();
							return TRUE;
						}
						return FALSE;
					}
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_CHECK_BackGround)) 
					{
						if (IsDlgButtonChecked(hWnd, IDC_CHECK_BackGround) == BST_UNCHECKED)
						{
							SetLayeredWindowAttributes(hWnd_clock, RGB(255, 255, 255), 255, LWA_ALPHA);
							SendMessage((HWND)lParam, BM_SETCHECK, 1, 0);
							return TRUE;
						}
						else 
						{
						
							SetLayeredWindowAttributes(hWnd_clock, RGB(255, 255, 255), 255, LWA_COLORKEY);
							SendMessage((HWND)lParam, BM_SETCHECK, 0, 0);
							return TRUE;
						}
						
						return TRUE;
					}
					return FALSE;
				}
			}
			return FALSE;
		}
		case WM_CTLCOLORSTATIC: 
		{
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_STATIC4)) 
			{
				HBRUSH hBursh = (HBRUSH)CreateSolidBrush(RGB(RGB_R2, RGB_G2, RGB_B2));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (INT_PTR)hBursh;
				
			}
			return FALSE;
		}
		case WM_CLOSE: 
		{
			EndDialog(hWnd, 1);
			return TRUE;
		}
		
	}
	return FALSE;
}

void NoBackGround(const WCHAR* imagePath, const WCHAR* maskPath)
{
	//在桌面显示图片，使用了模板
	HDC hdc, hdcDesktop;
	hdcDesktop = GetDC(FindWindow(NULL, TEXT("FolderView")));
	hdc = CreateCompatibleDC(NULL);
	HANDLE hbitmap = LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HANDLE hMask = LoadImage(NULL,maskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(hdc, hbitmap);
	BitBlt(hdcDesktop, 0, 0, 1920, 1500, hdc, 0, 0, SRCINVERT);
	SelectObject(hdc, hMask);
	BitBlt(hdcDesktop, 0, 0, 1920, 1500, hdc, 0, 0, SRCAND);
	SelectObject(hdc, hbitmap);
	BitBlt(hdcDesktop, 0, 0, 1920, 1500, hdc, 0, 0, SRCINVERT);
	DeleteDC(hdc);
	DeleteDC(hdcDesktop);
}