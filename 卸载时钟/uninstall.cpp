#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

void DeleteAutoRun();
BOOL isHaveValue();
#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// 使用WinMain是因为他不会弹出命令窗口
	//弹出对话框提示是否确认删除
	if (MessageBox(NULL, L"确认卸载？", L"确认？", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
	{
		//检测程序是否在运行中 -----日后升级
		//先将注册表中的数据删除
		if (isHaveValue())
		{
			DeleteAutoRun();
		}
		//将主程序删除
		WCHAR FileName[_MAX_FNAME];
		GetModuleFileName(NULL, FileName, _MAX_FNAME);
		PathRemoveFileSpec(FileName);
		wcscat(FileName, L"\\时钟2.exe");
		if (_wremove(FileName) == 0) 
		{
			MessageBox(NULL, L"卸载成功", L"提示", MB_OK);
		}
		else 
		{
			MessageBox(NULL, L"卸载失败", L"提示", MB_OK);
		}
		
		//将本程序删除
		GetModuleFileName(NULL, FileName, _MAX_FNAME);
		WCHAR Command[_MAX_PATH] = L"/C ping 1.1.1.1 -n 1 -w 1  & del ";
		wcscat(Command, FileName);
		ShellExecute(NULL, L"open", L"cmd.exe", Command, NULL, SW_HIDE);

	}
	else 
	{
		return 0;
	}
}
		
	
	


void DeleteAutoRun()
{
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, AUTOPATH, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	RegDeleteValue(hKey, L"clock");
	RegCloseKey(hKey);
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