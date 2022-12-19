#pragma once
#include <Windows.h>
#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
void SetAutoRun();
void DeleteAutoRun();
BOOL isHaveValue();