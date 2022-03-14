#pragma once
#include "Windows.h"
#define WINDWS_NAME "TestOn"
BOOL ListProcessModules(DWORD dwPID);
HMODULE getModelBaseAddr(DWORD dwPID, LPCSTR modelName);
VOID DrawInfo(DWORD pid, HANDLE hprocess);
VOID MegText(const char* Text);
BOOL CreateWindows(HINSTANCE hPrevInstance, HWND& Out_Hwnd);
void MsgRecv();

static BOOL IsGamgProcessRun = FALSE;     

static Window* g_WindowObject = NULL; //窗口对象
static DrawDx* g_DxObject = NULL;   //DX对象

/*    game info    */
static LPCSTR g_WindowClass = "RiotWindowClass";
static LPCSTR g_WindowName = "League of Legends (TM) Client";
static LPCSTR g_ModelName = "League of Legends.exe";

/*    window data               */
static HWND g_GameHwnd = NULL; 
static DWORD g_ProcessID = 0; 
static HANDLE g_HandleProcess; 

/*    addrs data        */
static HMODULE g_ModelAddr; 
static DWORD g_ViewHightAddr; 
static DWORD g_WindowXAddr;
static DWORD g_WindowYAddr;
static DWORD g_MyselfXAddr;
static DWORD g_MyselfYAddr;
static DWORD g_MyselfZAddr;

// thread 
DWORD WINAPI OnServer(LPVOID lp);  
DWORD WINAPI OnGameRun(LPVOID lp);