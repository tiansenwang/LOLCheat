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
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer), &Buffer, 4, NULL);
	//sprintf(text, "%d", Buffer);
	//MegText(text);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xC), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xBC), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x20), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0x4), &Buffer, 4, NULL);
	return Buffer + 0x268;
}

VOID DrawInfo(DWORD pid, HANDLE hprocess) {
	// 遍历进程模块
	LPCSTR model = "League of Legends.exe";
	HMODULE modelAddr = getModelBaseAddr(pid, model);
	printf("modelAddr : 0x%08X\n", modelAddr);
	DWORD baseAddr = (DWORD)modelAddr + 0x0185C0FC;
	printf("baseAddr + 一级偏移: 0x%08X\n", baseAddr);

	DWORD Buffer;
	ReadProcessMemory(hprocess, (LPCVOID)baseAddr, &Buffer, 4, NULL);
	printf("基地址+偏移读取 ： %x\n", Buffer);

	DWORD viewHightAddr;
	ReadProcessMemory(hprocess, (LPCVOID)(Buffer + 0xC), &viewHightAddr, 4, NULL);
	printf("二次间址：%x\n", viewHightAddr);

	// 最终的高度数据地址
	viewHightAddr += 0x26C;
	printf("viewHightAddr : %x\n", viewHightAddr);

	//读取内存数据 三次间址获取高度视距
	float viewHight;
	ReadProcessMemory(hprocess, (LPCVOID)viewHightAddr, &viewHight, 4, NULL);
	printf("viewHight : %f\n", viewHight);

	// 写入内存数据
	viewHight = 4000.0;
	WriteProcessMemory(hprocess, (LPVOID)viewHightAddr, &viewHight, 4, NULL);
	printf("修改高度视距为4000");
}

FLOAT GetViewHight() {
	FLOAT viewHight;
	ReadProcessMemory(g_HandleProcess, (LPCVOID)g_ViewHightAddr, &viewHight, 4, NULL);
	return viewHight;
}

VOID WriteViewHight(float viewHight) {
	WriteProcessMemory(g_HandleProcess, (LPVOID)g_ViewHightAddr, &viewHight, 4, NULL);
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
		::TranslateMessage(&msg);   //将得到的消息翻译
		::DispatchMessage(&msg);    //翻译来的消息发送
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

	while (1)
	{
		g_GameHwnd = FindWindow(g_WindowClass, g_WindowName);
		if (GetAsyncKeyState(VK_F6) || g_GameHwnd == NULL)exit(0);

		FLOAT viewHight = GetViewHight();
		if (GetAsyncKeyState(VK_F1)) { // press f1 add viewhight
			viewHight += 500.0;
			WriteViewHight(viewHight);
			Sleep(500);
		}
		if (GetAsyncKeyState(VK_F5)) { // info erro try refresh 
			g_ViewHightAddr = GetViewHightAddr(g_ModelAddr, g_HandleProcess);
		}
		// draw info 
		g_DxObject->MoveWindowAndDraw(g_GameHwnd, MyHwnd);
		g_DxObject->DxDrawStart();

		g_DxObject->DxDrawText(5, 10, L"按  下  F6  结  束  程  序", D2D1::ColorF::Red, g_WindowObject->GetMyRect(), 10.0f);
		g_DxObject->DxDrawText(5, 30, L"游  戏  对  局  开  始", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(5, 50, L"按  下  F5  刷  新  数  据", D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		g_DxObject->DxDrawText(5, 70, L"按  下  F1  增  加  视  距  高  度", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		g_DxObject->DxDrawText(5, 90, L"当前高度 ：", D2D1::ColorF::Red, g_WindowObject->GetMyRect());
		char hight[] = "";
		sprintf(hight, "%f", viewHight);
		g_DxObject->DxDrawText(80, 90, CharToWchar(hight), D2D1::ColorF::Red, g_WindowObject->GetMyRect());

		g_DxObject->DxEndDraw();
	}
}
