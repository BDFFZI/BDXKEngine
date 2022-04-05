#include "WindowBase.h"


WindowBase::WindowBase(PCWSTR windowName)
{
	this->windowName = windowName;
	this->hwnd = NULL;
}

void WindowBase::Show()
{
	//�����ϵͳע�ᴰ����
	WNDCLASS windowClass = {};
	windowClass.lpszClassName = windowName;//��������ʶ
	windowClass.lpfnWndProc = WindowProcess;//������Ϣ������
	RegisterClass(&windowClass);

	//��������
	HWND hwnd = CreateWindowEx(
		0,//������Ϊ
		windowName, //����������
		windowName,//���ڱ���
		WS_OVERLAPPEDWINDOW,//������ʽ
		CW_USEDEFAULT, CW_USEDEFAULT,//����λ��xy
		CW_USEDEFAULT, CW_USEDEFAULT,//���ڴ�Сxy
		NULL,//������
		NULL,//���ڲ˵�
		NULL,//ʵ�����,Ĭ��
		this//���ӵ�����
	);

	//��ʾ����
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

PCWSTR WindowBase::GetWindowName()
{
	return windowName;
}

LRESULT CALLBACK WindowBase::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}

LRESULT CALLBACK WindowBase::WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	WindowBase* window = NULL;

	//��ȡ����ʵ��
	if (messageSign == WM_NCCREATE)
	{
		//��һ����ת��ʵ��
		CREATESTRUCT* createStruct = (CREATESTRUCT*)lparameter;
		window = (WindowBase*)createStruct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->hwnd = hwnd;
	}
	else
	{
		window = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (window != NULL)
	{
		//ʹ�ô���ʵ���ṩ����Ϣ�������
		return window->HandleMessage(messageSign, wparameter, lparameter);
	}
	else
	{
		return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
	}
}