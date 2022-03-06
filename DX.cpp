#include "DX.h"

DrawDx* DrawDx::NewDraw()
{
	DrawDx* pDrawDx = new DrawDx();
	return pDrawDx;
}

BOOL DrawDx::DxInit(HWND Hwnd)
{
	try
	{
		if (!pRender)
		{
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
			pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(Hwnd, D2D1::SizeU(1024, 768)), &pRender);
			pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));

		}
	}
	catch (...)
	{
		return FALSE;
	}
	pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &DrawBrush);
	pWriteFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"MyFont", &pTextFormatKaiTi);
	return TRUE;

}

void DrawDx::DxDrawLine(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha)
{
	try
	{

		//pRender->Clear(D2D1_COLOR_F{ 1, 0, 0, 0 }); 纪念一下 我调试一晚上 就是这个  我刚开始封装的时候忘记删除了 然后一晚上 无限画不上方块 
		DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
		pRender->DrawLine(D2D1::Point2F((float)X, (float)Y), D2D1::Point2F((float)X + (float)XX, (float)Y), DrawBrush, Width);

	}
	catch (...)
	{
		MessageBox(NULL, TEXT("未知错误"), TEXT("在DxDrawLine里"), 0);
	}
}

void DrawDx::DxDrawText(int X, int Y, const WCHAR* text, DWORD Color, RECT MyRect, float Alpha)
{
	try
	{
		DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
		pRender->DrawText(text, wcslen(text), pTextFormatKaiTi,
			D2D1::RectF((float)X, (float)Y, (float)MyRect.right, (float)MyRect.bottom), DrawBrush);
	}
	catch (...)
	{
		MessageBox(NULL, TEXT("未知错误"), TEXT("在DxDrawText里"), 0);
	}
}

void DrawDx::DxTransverseline(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha)
{
	try
	{
		DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
		pRender->DrawLine(D2D1::Point2F((float)X, (float)Y), D2D1::Point2F((float)X, (float)Y + YY), DrawBrush, Width);
	}
	catch (...)
	{
		MessageBox(NULL, TEXT("未知错误"), TEXT("在DxTransverseline里"), 0);
	}
}

void DrawDx::DxDrawBox(DWORD left, DWORD Top, DWORD Width, DWORD Height, DWORD Color, DWORD LineWidth)
{
	try
	{
		DxDrawLine(left, Top, Width, LineWidth, Color, 2, 15);
		DxDrawLine(left, Top + Height, Width, LineWidth, Color, 2, 15);
		DxTransverseline(left, Top - 1, LineWidth, Height + 2, Color, 2, 15);
		DxTransverseline(left + Width, Top - 1, LineWidth, Height + 2, Color, 2, 15);
	}
	catch (...)
	{
		MessageBox(NULL, TEXT("未知错误"), TEXT("在DrawBox里"), 0);
	}
}


void DrawDx::DrawRenderTextA(int X, int Y, const char* text, DWORD Color, RECT MyRect, float Alpha)
{
	WCHAR WStr[256] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, text, strlen(text), WStr, strlen(text) * 2);
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawText(WStr, wcslen(WStr), pTextFormatKaiTi, D2D1::RectF((float)X, (float)Y, (float)MyRect.right, (float)MyRect.bottom), DrawBrush);
}

void DrawDx::MoveWindowAndDraw(HWND GameHwnd, HWND MyHwnd)
{
	RECT tSize;
	DWORD dwStyle = GetWindowLong(GameHwnd, GWL_STYLE);
	if ((dwStyle & WS_BORDER) != 0)
	{
		GetWindowRect(GameHwnd, &tSize);
		int x = GetSystemMetrics(32);
		int y = GetSystemMetrics(33) + GetSystemMetrics(4);
		tSize.left += x;
		tSize.top += y;
		WindowX = (tSize.right - tSize.left - GetSystemMetrics(32));
		WindowY = tSize.bottom - tSize.top - GetSystemMetrics(33);
		MoveWindow(MyHwnd, tSize.left, tSize.top, WindowX, WindowY, TRUE);
		SetWindowPos(MyHwnd, HWND_TOPMOST, 0, 0, 0, 0, 3);
		pRender->Resize(D2D1::SizeU(WindowX, WindowY));
	}
	else
	{
		WindowX = GetSystemMetrics(SM_CXSCREEN);
		WindowY = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(MyHwnd, 0, 0, WindowX, WindowY, TRUE);
		SetWindowPos(MyHwnd, HWND_TOPMOST, 0, 0, 0, 0, 3);
		pRender->Resize(D2D1::SizeU(WindowX, WindowY));
	}
}

void DrawDx::DxDrawStart()
{
	pRender->BeginDraw();
	pRender->Clear(D2D1_COLOR_F{ 1, 0, 0, 0 });
}

void DrawDx::DxEndDraw()
{
	pRender->EndDraw();
}
