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
��д��:BuleBigDream
�ο�:CSDN WinMain��׼ģ��
----------------------------------------------
Window����Ҫ���ڴ������� �� ���ڵ���Ϣ����
*/


class Window
{
public:
	/*
	��������Window
	����:Ĭ�Ϲ��캯��
	����1:WinMain����Ĳ���hInstance
	����2:��ʾ��ʽ.Ĭ��ΪSW_SHOW
	���أ��޷���ֵ
	*/
	Window(HINSTANCE hInstance, int Show = SW_SHOW);

	/*
	������:NewWindow
	����:ΪWindow�����ռ�
	����1:WinMain����Ĳ���hInstance
	����2:��ʾ��ʽ.Ĭ��ΪSW_SHOW
	����:������Ķ���ָ��
	*/
	static Window* NewWindow(HINSTANCE hInstance, int Show = SW_SHOW);


public:
	/*
	��������MyRegisterClassEx
	����:ע�ᴰ��
	����1:��������
	����2:�ص�����
	����:��������ֵ
	*/
	BOOL MyRegisterClassEx(const char* WindowClassName, WNDPROC lpfnWndProc);

	/*
	������:MyCreateWindow
	����:Ĭ�ϴ���һ��͸���Ĵ���
	����1:������
	����2:�������ڵ�X����
	����3:�������ڵ�y����
	����4:���ڿ��
	����5:���ڸ߶�
	����6:���ڷ�� Ĭ��͸�� �ö�
	����:�´��ڵľ��,ʧ�ܷ���0;
	*/
	HWND MyCreateWindow(const char* WindowName, int x, int y, int Width, int Hght, DWORD WinowStly = WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

	/*
	������:GetErrorCode
	����:��ȡ�������
	����:���ش������

	*/
	int GetErrorCode();


	/*
	������:GetMyRect
	����:��ȡ�����ڵĳ����
	����:RECT��������ṹ
	*/
	RECT GetMyRect();

private:
	HINSTANCE hInst;
	int nCmdShow;
	int ErrorCode; //��¼���൱ǰ�����Ĵ���
	const char* WindowClassName;
	RECT MyRect;
};