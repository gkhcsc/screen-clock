#include "clock.h"
//测试推送
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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wndclass);
	hWnd_clock = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_LAYERED, szAppName, TEXT("Alarm clock"), WS_POPUP, 1200, 20, 200, 50, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd_clock, iCmdShow);
	UpdateWindow(hWnd_clock);
	SetWindowPos(hWnd_clock, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);
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
			RegisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW, MOD_ALT | MOD_CONTROL, 'F');

			hWnd_desktop = FindWindow(NULL, TEXT("FolderView"));
			SetParent(hWnd_clock, hWnd_desktop);
		
			hWnd_time = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, 260, 129, hWnd, NULL, hInstance, NULL);
			hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
			hMenu = GetSubMenu(hMenu, 0);
			hFont = CreateFont(-30, -15, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, TEXT("微软雅黑"));
			SetWindowLong(hWnd_clock, GWL_STYLE, WS_CHILD);
			SetTimer(hWnd, 0, 900, NULL);
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
						case ID_MENU_40004:
						{
							
							UnregisterHotKey(hWnd, ID_HOTKEY_HIDEORSHOW);
							Shell_NotifyIcon(NIM_DELETE, &nid);
							PostQuitMessage(0);
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
				SetForegroundWindow(hWnd);
		
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
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
			if (wParam == ID_HOTKEY_HIDEORSHOW)
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
			return 0;

		}
		case WM_DESTROY:
		{
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
	switch (msg)
	{
		case WM_INITDIALOG: 
		{
			if (isHaveValue() == TRUE)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_SETCHECK, 1, 0);
			}
			DWORD flag;
			GetLayeredWindowAttributes(hWnd_clock, NULL, NULL, &flag);
			if (flag == LWA_ALPHA) 
			{
				SendMessage(GetDlgItem(hWnd, IDC_CHECK2), BM_SETCHECK, 1, 0);
			}
			return TRUE;
		}
		case WM_COMMAND: 
		{
			switch (HIWORD(wParam))
			{
				case EN_CHANGE: 
				{
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
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_CHECK1)) 
					{
						if (IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_UNCHECKED)
						{
							if (isHaveValue() == FALSE)
							{
								SendMessage((HWND)lParam, BM_SETCHECK, 1, 0);
								AutoRun();
								return TRUE;
							}
							return FALSE;
						}
							

						if (IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED) 
						{
							SendMessage((HWND)lParam, BM_SETCHECK, 0, 0);
							DeleteAutoRun();
							return TRUE;
						}
						return FALSE;
					}
					if ((HWND)lParam == GetDlgItem(hWnd, IDC_CHECK2)) 
					{
						if (IsDlgButtonChecked(hWnd, IDC_CHECK2) == BST_UNCHECKED)
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

BOOL isHaveValue() 
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, AUTOPATH, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	if (RegGetValue(HKEY_CURRENT_USER, AUTOPATH, L"clock", RRF_RT_REG_SZ, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

void AutoRun()
{
	HKEY hKey;
	TCHAR szPath[MAX_PATH];
	RegOpenKeyEx(HKEY_CURRENT_USER, AUTOPATH, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	GetModuleFileName(NULL, szPath, MAX_PATH);
	RegSetValueEx(hKey, L"clock", 0, REG_SZ, (BYTE*)szPath, MAX_PATH);
	RegCloseKey(hKey);
}

void DeleteAutoRun()
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, AUTOPATH, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	RegDeleteValue(hKey, L"clock");
	RegCloseKey(hKey);
}

