#include "WindowBase.h"
#include <iostream>
using namespace std;

WindowBase::WindowBase(PCWSTR windowName)
{
	WindowName = windowName;
	hwnd = NULL;
}

BOOL WindowBase::Show()
{
	//�����ϵͳע�ᴰ����
	WNDCLASS windowClass = {};
	windowClass.lpszClassName = WindowName;//��������ʶ
	windowClass.lpfnWndProc = WindowProcess;//������Ϣ������
	RegisterClass(&windowClass);

	//��������
	hwnd = CreateWindowEx(
		0,//������Ϊ
		WindowName, //����������
		WindowName,//���ڱ���
		WS_OVERLAPPEDWINDOW,//������ʽ
		CW_USEDEFAULT, CW_USEDEFAULT,//����λ��xy
		CW_USEDEFAULT, CW_USEDEFAULT,//���ڴ�Сxy
		NULL,//������
		NULL,//���ڲ˵�
		NULL,//ʵ�����,Ĭ��
		this//���ӵ�����
	);
	if (hwnd != NULL)
	{
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		return TRUE;
	}

	return FALSE;
}

PCWSTR WindowBase::GetWindowName()
{
	return WindowName;
}

HWND WindowBase::GetHWND()
{
	return hwnd;
}

LRESULT WindowBase::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_HOTLIGHT));

		EndPaint(hwnd, &ps);
	}
	return 0;
	}
}

LRESULT WindowBase::WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	WindowBase* window = NULL;

	//��ȡ����ʵ��
	if (messageSign == WM_NCCREATE)
	{
		//��һ����ת��ʵ��
		CREATESTRUCT* createStruct = (CREATESTRUCT*)lparameter;
		window = (WindowBase*)createStruct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
	}
	else
	{
		window = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (window != NULL)
	{
		//ʹ�ô���ʵ���ṩ����Ϣ�������
		return	window->HandleMessage(messageSign, wparameter, lparameter);
	}
	else
	{
		return	DefWindowProc(hwnd, messageSign, wparameter, lparameter);
	}
}