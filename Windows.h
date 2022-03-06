#pragma once
#pragma once
#include<windows.h>
#include <Dwrite.h>
#include<Uxtheme.h>
#include<dwmapi.h>
#pragma comment( lib,"GDI32.lib" )
#pragma comment( lib, "user32.lib" )
#define NULL 0
#define FALSE 0
/*
----------------------------------------------
编写者:BuleBigDream
参考:CSDN WinMain标准模板
----------------------------------------------
Window类主要用于创建窗口 和 窗口的消息处理
*/


class Window
{
public:
	/*
	函数名：Window
	功能:默认构造函数
	参数1:WinMain传入的参数hInstance
	参数2:显示方式.默认为SW_SHOW
	返回：无返回值
	*/
	Window(HINSTANCE hInstance, int Show = SW_SHOW);

	/*
	函数名:NewWindow
	功能:为Window类分配空间
	参数1:WinMain传入的参数hInstance
	参数2:显示方式.默认为SW_SHOW
	返回:返回类的对象指针
	*/
	static Window* NewWindow(HINSTANCE hInstance, int Show = SW_SHOW);


public:
	/*
	函数名：MyRegisterClassEx
	功能:注册窗口
	参数1:窗口类名
	参数2:回调函数
	返回:返布尔型值
	*/
	BOOL MyRegisterClassEx(const char* WindowClassName, WNDPROC lpfnWndProc);

	/*
	函数名:MyCreateWindow
	功能:默认创建一个透明的窗口
	参数1:窗口名
	参数2:窗口所在的X坐标
	参数3:窗口所在的y坐标
	参数4:窗口宽度
	参数5:窗口高度
	参数6:窗口风格 默认透明 置顶
	返回:新窗口的句柄,失败返回0;
	*/
	HWND MyCreateWindow(const char* WindowName, int x, int y, int Width, int Hght, DWORD WinowStly = WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

	/*
	函数名:GetErrorCode
	功能:获取错误代码
	返回:返回错误代码

	*/
	int GetErrorCode();


	/*
	函数名:GetMyRect
	功能:获取本窗口的长宽高
	返回:RECT窗口坐标结构
	*/
	RECT GetMyRect();

private:
	HINSTANCE hInst;
	int nCmdShow;
	int ErrorCode; //记录本类当前发生的错误。
	const char* WindowClassName;
	RECT MyRect;
};