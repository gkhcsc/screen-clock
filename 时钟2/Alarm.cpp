#include "Alarm.h"
int g_index_hours = -1, g_index_min = -1, g_index_sec = -1;
HANDLE hProcess;
time_t timeDifference(int hour, int minute, int sec) {
	struct tm* CurrentTime;
	time_t AfterTotalSeconds, CurrentTotalTime;
	time_t i;
	i = time(NULL);
	CurrentTime = localtime(&i);
	AfterTotalSeconds = hour * 3600 + minute * 60 + sec;
	CurrentTotalTime = CurrentTime->tm_hour * 3600 + CurrentTime->tm_min * 60 + CurrentTime->tm_sec;
	return AfterTotalSeconds - CurrentTotalTime;
}

HANDLE PlayAudio(LPCWSTR ffplaypath, LPCWSTR AudioPath) {
	//hProcess = PlayAudio(TEXT("D:\\installation path\\ffmpeg\\bin\\ffplay.exe"), TEXT(" a.mp3 -nodisp -loop 0"));//�ڶ�������ǰһ��Ҫ�пո�
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	LPCWSTR lpParameter = AudioPath;
	CreateProcess(ffplaypath, (LPWSTR)lpParameter, 0, 0, 0, CREATE_NO_WINDOW, 0, 0, &si, &pi);
	return pi.hProcess;
}

BOOL CALLBACK AlarmProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd_button;
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		WCHAR num[3] = { 0 };
		for (int i = 1; i < 25; i++)   //�����ʱ��ĺ���ԭ��Сʱ����Ϊ0
		{
			wsprintf(num, L"%d", i);
			SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_ADDSTRING, (WPARAM)i - 1, (LPARAM)num);
		}
		for (int i = 0; i < 60; i++)
		{
			wsprintf(num, L"%d", i);
			SendMessage(GetDlgItem(hWnd, IDC_COMBO2), CB_ADDSTRING, (WPARAM)i - 1, (LPARAM)num);
		}
		for (int i = 0; i < 60; i++)
		{
			wsprintf(num, L"%d", i);
			SendMessage(GetDlgItem(hWnd, IDC_COMBO3), CB_ADDSTRING, (WPARAM)i - 1, (LPARAM)num);
		}
		SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, (WPARAM)g_index_hours, 0);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO2), CB_SETCURSEL, (WPARAM)g_index_min, 0);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO3), CB_SETCURSEL, (WPARAM)g_index_sec, 0);
		return TRUE;
	}
	case WM_TIMER:
	{
		hProcess = PlayAudio(TEXT("ffplay.exe"), TEXT(" audio.mp3 -nodisp -loop 5")); //δ�����Խ���Ƶ�ļ�����һ���������ļ����ڣ��ú��ȡ�ļ��е������ļ������������ļ�����һ���б�����û��Լ�ѡ��
		KillTimer(hWnd, wParam);
		g_index_hours = -1;
		g_index_min = -1;
		g_index_sec = -1;
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		{
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_BUTTON1))
			{
				WCHAR buffer[1024] = { 0 };
				WCHAR buffer2[1024] = { 0 };
				int index_hours, index_min, index_sec, timeRemaining;
				index_hours = SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);
				index_min = SendMessage(GetDlgItem(hWnd, IDC_COMBO2), CB_GETCURSEL, 0, 0);
				index_sec = SendMessage(GetDlgItem(hWnd, IDC_COMBO3), CB_GETCURSEL, 0, 0);
				if (index_hours == -1 && index_min == -1 && index_sec == -1)
				{
					MessageBox(0, L"����ѡ��", 0, 0);
					return TRUE;
				}
				else if (g_index_hours != index_hours || g_index_min != index_min || g_index_sec != index_sec)
				{
					timeRemaining = timeDifference(index_hours + 1, index_min, index_sec);
					g_index_hours = index_hours;
					g_index_min = index_min;
					g_index_sec = index_sec;
					KillTimer(0, ID_ALARM); //ɾ����ǰ�ļ�ʱ��
					if (SetTimer(0, ID_ALARM, timeRemaining * 1000, (TIMERPROC)AlarmProc) != 0) //�����µļ�ʱ��
					{
						MessageBox(0, L"���óɹ�", 0, 0);
						return TRUE;
					}
					else
					{
						MessageBox(0, L"������ʱ��ʧ��", 0, 0);
						return TRUE;
					}
					return TRUE;
				}
				else
				{
					MessageBox(0, L"�벻Ҫ�ظ�����", 0, 0);
					return TRUE;
				}

				return FALSE;
			}
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_BUTTON2))
			{
				KillTimer(0, ID_ALARM);
				g_index_hours = -1;
				g_index_min = -1;
				g_index_sec = -1;
				SendMessage(GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, (WPARAM)g_index_hours, 0);
				SendMessage(GetDlgItem(hWnd, IDC_COMBO2), CB_SETCURSEL, (WPARAM)g_index_min, 0);
				SendMessage(GetDlgItem(hWnd, IDC_COMBO3), CB_SETCURSEL, (WPARAM)g_index_sec, 0);
				MessageBox(0, L"ɾ���ɹ�", 0, 0);
				return TRUE;
			}


			return FALSE;
		}


		}
		return FALSE;
	}
	case WM_CLOSE:
	{
		EndDialog(hWnd, 1);
		return TRUE;
	}
	}
	return FALSE;
}

VOID _TerminateProcess() 
{
	TerminateProcess(hProcess, 0);
}
