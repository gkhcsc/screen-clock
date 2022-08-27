#pragma once
#include <Windows.h>
#include <time.h>
#include "resource.h"
#define WM_TRAY (WM_USER + 100)
#define ID_HOTKEY_HIDEORSHOW 1
#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

HWND hWnd_clock, hWnd_desktop, hWnd_time;
NOTIFYICONDATA nid;
int RGB_R = 0;
int RGB_G = 255;
int RGB_B = 0;

int RGB_R2 = 0;
int RGB_G2 = 255;
int RGB_B2 = 0;

void AutoRun();
void DeleteAutoRun();
BOOL isHaveValue();
void InitTray(HINSTANCE hInstance, HWND hWnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SettingProc(HWND, UINT, WPARAM, LPARAM);