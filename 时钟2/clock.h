#pragma once
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <atlimage.h>
#include "resource.h"
#define WM_TRAY (WM_USER + 100)
#define ID_HOTKEY_HIDEORSHOW 1
#define ID_HOTKEY_CLOSEALARM 2
#define ID_HOTKEY_LEFT 3
#define ID_HOTKEY_RIGHT 4
#define ID_HOTKEY_UP 5
#define ID_HOTKEY_DOWM 6

#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define ID_ALARM 1


HWND hWnd_clock, hWnd_desktop, hWnd_time;
NOTIFYICONDATA nid;
int RGB_R = 0;
int RGB_G = 255;
int RGB_B = 0;

int RGB_R2 = 0;
int RGB_G2 = 255;
int RGB_B2 = 0;

int MovePrePixels = 3;
void SetAutoRun();
void DeleteAutoRun();
BOOL isHaveValue();
void InitTray(HINSTANCE hInstance, HWND hWnd);
time_t timeDifference(int hour, int minute,int sec);
HANDLE PlayAudio(LPCWSTR ffplaypath, LPCWSTR AudioPath);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SettingProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AlarmProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void NoBackGround(const WCHAR* imagePath, const WCHAR* maskPath);