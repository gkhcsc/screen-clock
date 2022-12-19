#include "config.h"

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

void SetAutoRun()
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