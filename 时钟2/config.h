#pragma once
#include <Windows.h>

#define REFRESH_RATE 800

#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
void SetAutoRun();
void DeleteAutoRun();
BOOL isHaveValue();