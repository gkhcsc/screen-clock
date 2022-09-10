#include "Alarm_BaseBox.h"
void Alarm_BaseBox::InitBox(HWND hWnd_parent, HINSTANCE hInstance, const WCHAR* BoxName, int x, int y, int nWidth, int nHeight)
{
	WCHAR szBaseBox[] = L"BaseBox";
	wndclass.lpfnWndProc = (WNDPROC)Alarm_BaseBox::BaseBoxProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szBaseBox;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wndclass);
	hWnd_main = CreateWindow(szBaseBox, BoxName, WS_CHILD | WS_VISIBLE, x, y, nWidth, nHeight, hWnd_parent, (HMENU)1, hInstance, 0);

}



void Alarm_BaseBox::showWindow()
{
	ShowWindow(hWnd_main, SW_SHOW);
	UpdateWindow(hWnd_main);
}

LRESULT CALLBACK Alarm_BaseBox::BaseBoxProc(HWND hWnd, INT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWnd_static_time, hWnd_button_switch;
	static HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	static HFONT hFont;
	RECT rect;
	switch (msg)
	{
	case WM_CREATE:
	{
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		int width_font = height / 100 * 18;
		int height_font = height / 100 * 30;
		hFont = CreateFont(-height_font, -width_font, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, TEXT("Î¢ÈíÑÅºÚ"));
		hWnd_static_time = CreateWindow(L"static", NULL, WS_CHILD | WS_VISIBLE, width / 50, (height - height_font) / 2, width / 5 * 3, height, hWnd, (HMENU)1, hInstance, 0);
		hWnd_button_switch = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 10, 5, hWnd, (HMENU)0, hInstance, NULL);
		SendMessage(hWnd_static_time, WM_SETFONT, (WPARAM)hFont, NULL);
		return 0;
	}
	case WM_CTLCOLORSTATIC:
	{
		HBRUSH hBursh = NULL;
		if ((HWND)lParam == hWnd_static_time)
		{
			SetWindowText(hWnd_static_time, L"20Ê±56·Ö");
		}
		return (INT_PTR)hBursh;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}