#pragma once
#include "resource.h"
#include <time.h>
#include <Windows.h>
#define ID_ALARM 1
time_t timeDifference(int hour, int minute, int sec);
HANDLE PlayAudio(LPCWSTR ffplaypath, LPCWSTR AudioPath);
VOID _TerminateProcess();
BOOL CALLBACK AlarmProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);