//
//		Win7 x64����������SDK��  by Thvoifar
//	https://bbs.pediy.com/thread-228522.htm
//
//


#include <tchar.h>
#include <windows.h>

// ȫ�ֱ����ͳ�ʼ��

HHOOK g_hMouse = NULL;// ��깳�Ӿ��
HHOOK g_hLowlevelMouse = NULL; // �ײ���깳�Ӿ��
HHOOK g_hKeyboard = NULL;// ���̹��Ӿ��
HHOOK g_hLowlevelKeyboard = NULL;// �ײ���̹��Ӿ��
#pragma data_seg("SharedSec")// ���ù����
HWND g_hWnd = NULL;// ���ݵ��ý��̵������ھ��
#pragma data_seg()


// ���ӹ��̺�����

LRESULT CALLBACK MouseProc(
	int code,       // hook code
	WPARAM wParam,  // virtual-key code
	LPARAM lParam   // keystroke-message information
	)
{
	return 1;
}

LRESULT CALLBACK LowLevelMouseProc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
	)
{
	return 1;
}

LRESULT CALLBACK KeyboardProc(
	int code,       // hook code
	WPARAM wParam,  // virtual-key code
	LPARAM lParam   // keystroke-message information
	)
{
	return 1;
}

LRESULT CALLBACK LowLevelKeyboardProc(
	int nCode,     // hook code
	WPARAM wParam, // message identifier
	LPARAM lParam  // pointer to structure with message data
	)
{
	PKBDLLHOOKSTRUCT pKey;
	pKey = (PKBDLLHOOKSTRUCT)lParam;
	// ����WIN CTRL ESC WIN+Tab Ctrl+ESC Ctrl+Shift+Esc Ctrl+Alt+Tab Ctrl+WIN+Tab WIN+U WIN+P WIN+X WIN+D WIN+E�ȼ�
	if (pKey->vkCode == VK_LWIN || pKey->vkCode == VK_RWIN || (GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_ESCAPE) & 0x8000))// ����WIN����Ctrl+ESC��ϼ�
	{
		return 1;
	}
	// ����Alt��ؼ� Alt+Tab 
	if (pKey->flags & LLKHF_ALTDOWN) // Alt����
	{
		if ('Q' == pKey->vkCode) // Alt+Q�˳�
		{
			UnhookWindowsHookEx(g_hMouse);
			UnhookWindowsHookEx(g_hLowlevelMouse);
			UnhookWindowsHookEx(g_hKeyboard);
			UnhookWindowsHookEx(g_hLowlevelKeyboard);
			SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		}
		return 1;
	}
	return CallNextHookEx(g_hLowlevelKeyboard, nCode, wParam, lParam);
}

// ����DLL�е���������

extern "C" _declspec(dllexport) BOOL __stdcall SetKeymsHook(HWND hwnd)
{
	g_hWnd = hwnd;
	HMODULE hModule = GetModuleHandle(_T("KeymsLockHook.dll")); //��ȡ��̬���ӿ�KeymsLockHook.dllģ����
	g_hMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, hModule, 0);
	if (NULL == g_hMouse)
	{
		MessageBox(NULL, _T("��װ��깳�ӳ���1��"), _T("error"), 0);
		return FALSE;
	}
	g_hLowlevelMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hModule, 0);
	if (NULL == g_hLowlevelMouse)
	{
		MessageBox(NULL, _T("��װ��깳�ӳ���2��"), _T("error"), 0);
		return FALSE;
	}
	g_hKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hModule, 0);
	if (NULL == g_hKeyboard)
	{
		MessageBox(NULL, _T("��װ���̹��ӳ���1��"), _T("error"), 0);
		return FALSE;
	}
	g_hLowlevelKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hModule, 0);
	if (NULL == g_hLowlevelKeyboard)
	{
		MessageBox(NULL, _T("��װ���̹��ӳ���2��"), _T("error"), 0);
		return FALSE;
	}
	return TRUE;
}