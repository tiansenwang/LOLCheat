#include "Windows.h"
#include "Windows.h"

Window::Window(HINSTANCE hInstance, int Show)
{
	this->hInst = hInstance;
	this->nCmdShow = Show;
}

Window* Window::NewWindow(HINSTANCE hInstance, int Show)
{
	Window* pWindow = new Window(hInstance, Show);
	if (pWindow != NULL) return pWindow;
	return NULL;
}


BOOL Window::MyRegisterClassEx(const char* WindowClassName, WNDPROC lpfnWndProc)
{
	HBRUSH hBrush = ::CreateSolidBrush(RGB(62, 71, 98));
	WNDCLASSEX wndclass;
	this->WindowClassName = WindowClassName;
	HINSTANCE Inst = this->hInst;
	if (!Inst)
	{
		ErrorCode = 30001;
		return FALSE;
	}
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = hBrush;
	wndclass.hCursor = 0;
	wndclass.hIcon = 0;
	wndclass.hIconSm = 0;
	wndclass.lpfnWndProc = lpfnWndProc;
	wndclass.lpszClassName = WindowClassName;
	wndclass.lpszMenuName = 0;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = Inst;
	return RegisterClassEx(&wndclass);
}

HWND Window::MyCreateWindow(const char* WindowName, int x, int y, int Width, int Hght, DWORD WinowStly)
{
	MARGINS Margin = { -1, -1, -1, -1 };
	HINSTANCE Inst = this->hInst;
	if (!Inst)
	{
		ErrorCode = 30001;
		return NULL;
	}
	HWND hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		this->WindowClassName,
		WindowName,
		WS_POPUP,
		x, y, Width, Hght,
		0,
		0,
		Inst,
		0);
	ShowWindow(hWnd, SW_SHOW);
	GetWindowRect(hWnd, &MyRect);
	if (!WinowStly)
	{
		SetLayeredWindowAttributes(hWnd, 0, 0, 1);
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
	}

	return hWnd;
}


int Window::GetErrorCode()
{
	return this->ErrorCode;
}

RECT Window::GetMyRect()
{
	return this->MyRect;
}
