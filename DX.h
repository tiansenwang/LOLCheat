#pragma once
#pragma once
#include <d2d1.h>
#include <Dwrite.h>
#include<Uxtheme.h>
#include<dwmapi.h>
#pragma comment( lib,"Dwmapi.lib" )
#pragma  comment(lib,"d2d1.lib")
#pragma  comment(lib,"Dwrite.lib")
#pragma comment( lib,"winmm.lib" )

#define YELLOW D2D1::ColorF::Yellow                                   //黄色
#define BLACK D2D1::ColorF::Black                                    //黑色
#define BULE  D2D1::ColorF::Blue                                    //蓝色
#define WHITE D2D1::ColorF::White                                  //白色
#define RED  D2D1::ColorF::Red                                    //红色
#define PINK D2D1::ColorF::Pink                                  //骚粉
#define VIOLET D2D1::ColorF::Violet                             //基佬紫
#define AQUA D2D1::ColorF::Aqua                                //水绿色
#define AliceBlue D2D1::ColorF::AliceBlue                     //爱丽丝蓝(我也很绝望这是什么蓝)
#define AntiqueWhite D2D1::ColorF::AntiqueWhite              //古董白

/*----------------------------------------
编写者:BuleBigDream
类名:Draw
功能:绘制图形
*----------------------------------------
*/
class DrawDx
{
public:
	/*
	功能:创建类的对象指针
	返回:类的对象指针
	*/
	static DrawDx* NewDraw();
public:
	/*
	功能:DX初始化
	返回:返回布尔型
	*/
	BOOL DxInit(HWND Hwnd);

	/*
	函数名:DrawLine
	功能:绘制直线
	参数1:起点X点
	参数2:起点Y点
	参数3:终点X点
	参数4:终点Y点
	参数5:颜色
	参数6:线条宽度
	参数7:透明度 默认 15
	*/
	void  DxDrawLine(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha = 15.0f);
	/*
	函数名 : DxDrawText
	功能:绘制文本
	参数1:开始的X点
	参数2:开始的Y点
	参数3:绘制的文本
	参数4:本窗口大小的结构 通过 窗口类 GetMyRect获得 类型 RECT
	参数5:透明度 默认1
	*/
	void  DxDrawText(int X, int Y, const WCHAR* text, DWORD Color, RECT MyRect, float Alpha = 1.0f);

	/*
	函数名: DxTransverseline
	功能: 绘制横线
	参数1:起点X点
	参数2:起点Y点
	参数3:终点X点
	参数4:终点Y点
	参数5:颜色参考Draw.h头文件
	参数6:高度
	参数7:透明度 15.0f
	*/
	void  DxTransverseline(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha = 15.0f);

	/*
	函数名:DrawBox
	功能:绘制方块
	参数1:左边
	参数2:顶边
	参数3:宽度
	参数4:高度
	参数5:颜色
	参数6:线宽
	*/
	void DxDrawBox(DWORD left, DWORD Top, DWORD Width, DWORD Height, DWORD Color, DWORD LineWidth);

	/*
	函数名:MoveWindowAndDraw
	功能:移动窗口和绘制区域
	参数1:游戏句柄
	参数2:本窗口句柄
	返回:无
	*/
	void DrawRenderTextA(int X, int Y, const char* text, DWORD Color, RECT MyRect, float Alpha = 1.0f);

	void MoveWindowAndDraw(HWND GameHwnd, HWND MyHwnd);

	void DxDrawStart(); //开始绘制

	void DxEndDraw();//结束绘制

public:

private:
	ID2D1HwndRenderTarget* pRender = NULL;
	ID2D1Factory* pFactory = NULL;
	ID2D1SolidColorBrush* pBrush = NULL;
	IDWriteFactory* pWriteFactory = NULL;
	IDWriteTextFormat* pTextFormatMsyh = NULL;
	ID2D1SolidColorBrush* DrawBrush = NULL;
	IDWriteTextFormat* pTextFormatKaiTi = NULL;
	int WindowX;
	int WindowY;
};