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

#define YELLOW D2D1::ColorF::Yellow                                   //��ɫ
#define BLACK D2D1::ColorF::Black                                    //��ɫ
#define BULE  D2D1::ColorF::Blue                                    //��ɫ
#define WHITE D2D1::ColorF::White                                  //��ɫ
#define RED  D2D1::ColorF::Red                                    //��ɫ
#define PINK D2D1::ColorF::Pink                                  //ɧ��
#define VIOLET D2D1::ColorF::Violet                             //������
#define AQUA D2D1::ColorF::Aqua                                //ˮ��ɫ
#define AliceBlue D2D1::ColorF::AliceBlue                     //����˿��(��Ҳ�ܾ�������ʲô��)
#define AntiqueWhite D2D1::ColorF::AntiqueWhite              //�Ŷ���

/*----------------------------------------
��д��:BuleBigDream
����:Draw
����:����ͼ��
*----------------------------------------
*/
class DrawDx
{
public:
	/*
	����:������Ķ���ָ��
	����:��Ķ���ָ��
	*/
	static DrawDx* NewDraw();
public:
	/*
	����:DX��ʼ��
	����:���ز�����
	*/
	BOOL DxInit(HWND Hwnd);

	/*
	������:DrawLine
	����:����ֱ��
	����1:���X��
	����2:���Y��
	����3:�յ�X��
	����4:�յ�Y��
	����5:��ɫ
	����6:�������
	����7:͸���� Ĭ�� 15
	*/
	void  DxDrawLine(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha = 15.0f);
	/*
	������ : DxDrawText
	����:�����ı�
	����1:��ʼ��X��
	����2:��ʼ��Y��
	����3:���Ƶ��ı�
	����4:�����ڴ�С�Ľṹ ͨ�� ������ GetMyRect��� ���� RECT
	����5:͸���� Ĭ��1
	*/
	void  DxDrawText(int X, int Y, const WCHAR* text, DWORD Color, RECT MyRect, float Alpha = 1.0f);

	/*
	������: DxTransverseline
	����: ���ƺ���
	����1:���X��
	����2:���Y��
	����3:�յ�X��
	����4:�յ�Y��
	����5:��ɫ�ο�Draw.hͷ�ļ�
	����6:�߶�
	����7:͸���� 15.0f
	*/
	void  DxTransverseline(int X, int Y, int XX, int YY, DWORD Color, float Width, float Alpha = 15.0f);

	/*
	������:DrawBox
	����:���Ʒ���
	����1:���
	����2:����
	����3:���
	����4:�߶�
	����5:��ɫ
	����6:�߿�
	*/
	void DxDrawBox(DWORD left, DWORD Top, DWORD Width, DWORD Height, DWORD Color, DWORD LineWidth);

	/*
	������:MoveWindowAndDraw
	����:�ƶ����ںͻ�������
	����1:��Ϸ���
	����2:�����ھ��
	����:��
	*/
	void DrawRenderTextA(int X, int Y, const char* text, DWORD Color, RECT MyRect, float Alpha = 1.0f);

	void MoveWindowAndDraw(HWND GameHwnd, HWND MyHwnd);

	void DxDrawStart(); //��ʼ����

	void DxEndDraw();//��������

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