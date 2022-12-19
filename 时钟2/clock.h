#pragma once
#include "Alarm.h"
#include "config.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#include "resource.h"
#define WM_TRAY (WM_USER + 100)
#define ID_HOTKEY_HIDEORSHOW 1
#define ID_HOTKEY_CLOSEALARM 2
#define ID_HOTKEY_LEFT 3
#define ID_HOTKEY_RIGHT 4
#define ID_HOTKEY_UP 5
#define ID_HOTKEY_DOWM 6
#define WINDOW_WIDTH 200
#define WINDOW_HEIGH 50




HWND hWnd_clock, hWnd_desktop, hWnd_time;
NOTIFYICONDATA nid;
int RGB_R = 0;
int RGB_G = 255;
int RGB_B = 0;

int RGB_R2 = 0;
int RGB_G2 = 255;
int RGB_B2 = 0;

int MovePrePixels = 3;

void InitTray(HINSTANCE hInstance, HWND hWnd);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SettingProc(HWND, UINT, WPARAM, LPARAM);

void NoBackGround(const WCHAR* imagePath, const WCHAR* maskPath);

void DrawStr(Gdiplus::Graphics* g, int x, int y, const WCHAR m_Str[]);