#pragma once
#include <Windows.h>
class Alarm_BaseBox
{
	public:
		void InitBox(HWND hWnd_parent, HINSTANCE hInstance, const WCHAR* BoxName, int x, int y, int nWidth, int nHeight);
		void showWindow();
	private:
		HWND hWnd_main;
		WNDCLASS wndclass;
		static LRESULT CALLBACK BaseBoxProc(HWND hWnd, INT msg, WPARAM wParam, LPARAM lParam);
};

