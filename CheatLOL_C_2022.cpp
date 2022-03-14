#include <iostream>
#include <Windows.h>
#include <windows.h> 
#include "Windows.h"
#include <tlhelp32.h> 
#include <tchar.h> 
#include <string>
#include "DX.h"
#include "CheatLOL_C_2022.h"
#include "Utils.h"

using std::string;

BOOL ListProcessModules(DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		//printError(TEXT("CreateToolhelp32Snapshot (of modules)"));
		return(FALSE);
	}

	//  Set the size of the structure before using it. 
	me32.dwSize = sizeof(MODULEENTRY32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		//printError(TEXT("Module32First"));  // Show cause of failure 
		CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		return(FALSE);
	}

	//  Now walk the module list of the process, 
	//  and display information about each module 
	do
	{
		_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
		_tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
		_tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
		_tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
		_tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
		_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
		_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);

	} while (Module32Next(hModuleSnap, &me32));

	_tprintf(TEXT("\n"));

	//  Do not forget to clean up the snapshot object. 
	CloseHandle(hModuleSnap);
	return(TRUE);
}

HMODULE getModelBaseAddr(DWORD dwPID, LPCSTR modelName) {
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	me32.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hModuleSnap, &me32))
	{
		CloseHandle(hModuleSnap);
		return(FALSE);
	}
	do
	{
		if (!strcmp(me32.szModule, modelName)) {
			CloseHandle(hModuleSnap);
			//printf("MODULE NAME: %s\n", me32.szModule);
			return me32.hModule;
		}

	} while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
	return 0;
}

VOID MegText(const char* Text)
{
	MessageBox(NULL, Text, " ", 0);
}

HMODULE GetModelAddr(DWORD pid) {
	HMODULE modelAddr = getModelBaseAddr(pid, g_ModelName);
	return modelAddr;
}

DWORD GetViewHightAddr(HMODULE modelAddr, HANDLE hprocess) {
	//char text[] = "";
	DWORD Buffer = (DWORD)modelAddr + 0x3116374;
	//sprintf(text, "%d", Buffer);
//MegText(text);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xC), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xBC), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x20), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	return Buffer + 0x268;
}

VOID WriteViewHight(float viewHight) {
	WriteProcessMemory(g_HandleProcess, (LPVOID)g_ViewHightAddr, &viewHight, 4, NULL);
}

DWORD GetWindowYAddr(HMODULE modelAddr, HANDLE hprocess) {
	DWORD Buffer = (DWORD)modelAddr + 0x01879860;
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xC), &Buffer, 4, NULL);
	return Buffer + 0x15C;
}

DWORD GetMySelfX(HMODULE modelAddr, HANDLE hprocess) {
	DWORD Buffer = (DWORD)modelAddr + 0x01877334;
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x0), &Buffer, 4, NULL);
	return Buffer + 0x23C;
}

FLOAT GetFolatByAddr(DWORD Addr) {
	FLOAT value;
	ReadProcessMemory(g_HandleProcess, (LPCVOID)Addr, &value, 4, NULL);
	return value;
}

LRESULT  CALLBACK MyWinMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:  PostQuitMessage(0);	 break;
	}
	return  DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MsgRecv()
{
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

BOOL CreateWindows(HINSTANCE hPrevInstance, HWND& Out_Hwnd)
{
	BOOL nRet = FALSE;
	g_WindowObject = Window::NewWindow(hPrevInstance);
	if (g_WindowObject == NULL)
	{
		MegText("创建窗口失败");
		return nRet;

	}
	nRet = g_WindowObject->MyRegisterClassEx(WINDWS_NAME, MyWinMain);
	if (!nRet)
	{
		MegText("注册窗口失败");
		return nRet;
	}
	Out_Hwnd = g_WindowObject->MyCreateWindow(WINDWS_NAME, 0, 0, 768, 1024, 0);
	if (!Out_Hwnd)
	{
		MegText("注册窗口窗口创建失败");
		return nRet;
	}

	g_DxObject = DrawDx::NewDraw();
	if (g_DxObject == NULL)
	{
		MegText("创建DX对象失败");
		return nRet;
	}
	nRet = g_DxObject->DxInit(Out_Hwnd);
	if (!nRet)
	{
		MegText("DX对初始化失败");
		return nRet;
	}

	return nRet;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND MyWIndowHwnd = 0;
	BOOL nRet = CreateWindows(hInstance, MyWIndowHwnd);
	if (!nRet)
	{
		//exit(0);
	}
	else   //业务逻辑线程启动
	{
		CloseHandle(CreateThread(NULL, 0, OnServer, NULL, 0, 0));
		CloseHandle(CreateThread(NULL, 0, OnGameRun, MyWIndowHwnd, 0, 0));
		MsgRecv();
	}
	return 0;
}

DWORD WINAPI OnServer(LPVOID lp)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_F6)) // 监听F6按键按下后退出程序
		{
			exit(0);
		}
		g_GameHwnd = FindWindow(g_WindowClass, g_WindowName);
		if (g_GameHwnd != NULL) {
			IsGamgProcessRun = TRUE;
			break;
		}
		g_DxObject->DxDrawStart();
		g_DxObject->DxDrawText(5, 10, L"按  下  F6  结  束  程  序", D2D1::ColorF::Red, g_WindowObject->GetMyRect(), 10.0f);
		g_DxObject->DxDrawText(5, 25, L"游  戏  对  局  还  没  开  始", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxEndDraw();
	}
	return 0;
}
DWORD WINAPI OnGameRun(LPVOID lp) {

	HWND MyHwnd = (HWND)lp;
	while (1) // wait game begin
	{
		if (!IsGamgProcessRun) continue;
		break;
	}
	// window data
	g_GameHwnd = FindWindow(g_WindowClass, g_WindowName);
	GetWindowThreadProcessId(g_GameHwnd, &g_ProcessID);
	g_HandleProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_ProcessID);
	g_ModelAddr = GetModelAddr(g_ProcessID);

	// addr data
	g_ViewHightAddr = GetViewHightAddr(g_ModelAddr, g_HandleProcess);
	g_WindowYAddr = GetWindowYAddr(g_ModelAddr, g_HandleProcess);
	g_WindowXAddr = g_WindowYAddr - 8;
	g_MyselfXAddr = GetMySelfX(g_ModelAddr, g_HandleProcess);
	g_MyselfYAddr = g_MyselfXAddr + 8;
	g_MyselfZAddr = g_MyselfXAddr + 4;

	while (1)
	{
		g_GameHwnd = FindWindow(g_WindowClass, g_WindowName);
		if (GetAsyncKeyState(VK_F6) || g_GameHwnd == NULL)exit(0);


		FLOAT viewHight = GetFolatByAddr(g_ViewHightAddr);
		FLOAT windowX = GetFolatByAddr(g_WindowXAddr);
		FLOAT windowY = GetFolatByAddr(g_WindowYAddr);
		FLOAT myselfWordX = GetFolatByAddr(g_MyselfXAddr);
		FLOAT myselfWordY = GetFolatByAddr(g_MyselfYAddr);
		FLOAT myselfWordZ = GetFolatByAddr(g_MyselfXAddr);

		if (GetAsyncKeyState(VK_F1)) { // press f1 add viewhight
			viewHight += 500.0;
			WriteViewHight(viewHight);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F5)) { // info erro try refresh 
			g_ViewHightAddr = GetViewHightAddr(g_ModelAddr, g_HandleProcess);
			g_WindowYAddr = GetWindowYAddr(g_ModelAddr, g_HandleProcess);
			g_WindowXAddr = g_WindowYAddr - 8;
		}
		// draw info 
		g_DxObject->MoveWindowAndDraw(g_GameHwnd, MyHwnd);
		g_DxObject->DxDrawStart();

		g_DxObject->DxDrawText(5, 10, L"按  下  F6  结  束  程  序", D2D1::ColorF::Red, g_WindowObject->GetMyRect(), 10.0f);
		g_DxObject->DxDrawText(5, 30, L"游  戏  对  局  开  始", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(5, 50, L"按  下  F5  刷  新  数  据", D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		g_DxObject->DxDrawText(5, 70, L"按  下  F1  增  加  视  距  高  度", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(5, 90, L"当前高度 ：", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		char tmp[] = "";
		sprintf(tmp, "%f", viewHight);
		g_DxObject->DxDrawText(80, 90, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		sprintf(tmp, "%f", windowX);
		g_DxObject->DxDrawText(90, 120, L"Window X writable value : ", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(300, 120, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		sprintf(tmp, "%f", windowY);
		g_DxObject->DxDrawText(90, 140, L"Window Y writable value : ", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(300, 140, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		sprintf(tmp, "%f", myselfWordX);
		g_DxObject->DxDrawText(90, 160, L"Myself word X writable value : ", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(300, 160, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		sprintf(tmp, "%f", myselfWordY);
		g_DxObject->DxDrawText(90, 180, L"Myself word Y writable value : ", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(300, 180, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		sprintf(tmp, "%f", myselfWordZ);
		g_DxObject->DxDrawText(90, 200, L"Myself word Z writable value : ", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(300, 200, CharToWchar(tmp), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		g_DxObject->DxEndDraw();
	}
}
