#pragma once
//C++��������ʾ�����������ʹ��
//W ����wchar_t,�Դ���Unicode��utf-16,ע�����ڶ������
//L ����x16ʱ���Ĳ�����ڲ���Ҳ�ǳ�
//P ָ��
//LP ��ָ��
//C ����

//C++���⺯����׺���������Ĳ�ͬ�汾
// 
//������ʹ�õ��ַ�����
//A ASCII����
//W ���ַ����룬�����ľ������
// 
//һ�������ɫ�洢��ʽ
//U	�޷������� �磺0-255
//F ������ �磺0.0-1.0

//C++��������
//PCWSTR = �������ַ�ָ��,��׼�ַ���
//LRESULT = long int����ʾ�����ķ��ش���
//HWND = ���ھ��
//BOOL = int
//WPARAM = uint
//LPARAM = int

//�������ݽṹ
#include "String.h"
#include "Com.h"
#include "Color.h"
#include "Vector2.h"
#include "Math.h"
#include "Time.h"
//��չ����
#include "WindowBase.h"
#include "DialogWindow.h"
#include "MainWindow.h"
#include "Debug.h"
#include "Assert.h"
#include "GameObject.h"
#include "Component.h"

class BDXKEngine {
public:
	static void Initialize()
	{
		std::setlocale(LC_ALL, "zh-CN");
		WindowBase window = { L"BDXKEngine" };
		window.Show();
		Graphics::SetRenderTarget(window.GetHwnd());
		Time::Initialize();
	}

	static void Run()
	{
		while (true)
		{
			Time::BeginFrame();
			Graphics::BeginDraw();

			Graphics::ClearCanvas();

			for (GameObject* gameObject : GameObject::gameObjects)
			{
				gameObject->OnUpdate();
			}

			Graphics::EndDraw();
			Time::EndFrame();
		}
	}
};