#include <Windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

void DeleteAutoRun();
BOOL isHaveValue();
#define AUTOPATH  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// ʹ��WinMain����Ϊ�����ᵯ�������
	//�����Ի�����ʾ�Ƿ�ȷ��ɾ��
	if (MessageBox(NULL, L"ȷ��ж�أ�", L"ȷ�ϣ�", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
	{
		//�������Ƿ��������� -----�պ�����
		//�Ƚ�ע����е�����ɾ��
		if (isHaveValue())
		{
			DeleteAutoRun();
		}
		//��������ɾ��
		WCHAR FileName[_MAX_FNAME];
		GetModuleFileName(NULL, FileName, _MAX_FNAME);
		PathRemoveFileSpec(FileName);
		wcscat(FileName, L"\\ʱ��2.exe");
		if (_wremove(FileName) == 0) 
		{
			MessageBox(NULL, L"ж�سɹ�", L"��ʾ", MB_OK);
		}
		else 
		{
			MessageBox(NULL, L"ж��ʧ��", L"��ʾ", MB_OK);
		}
		
		//��������ɾ��
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