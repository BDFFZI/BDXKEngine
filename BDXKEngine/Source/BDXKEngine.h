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
#include <functional>
#include "String.h"
#include "Com.h"
#include "Color.h"
#include "Vector2.h"
#include "Math.h"
#include "Time.h"
#include "Matrix.h"
//��չ����
#include "WindowBase.h"
#include "Window.h"
#include "Debug.h"
#include "Assert.h"
#include "GameObject.h"
#include "Component.h"

class BDXKEngine {
public:
	static void Run(std::function<void()> onStart)
	{
		std::setlocale(LC_ALL, "zh-CN");

		//��ʼ��
		Window window = Window(L"BDXKEngine",
			//OnCreate
			[](Window* window) {
				Graphics::SetRenderTarget(window->GetHwnd());
				Time::Awake();
			},
			//OnPaint
				[](Window* window) {
				Time::BeginFrame();
				Graphics::BeginDraw();

				Graphics::ClearCanvas();

				for (GameObject* gameObject : GameObject::gameObjects)
				{
					gameObject->OnUpdate();
				}

				Graphics::EndDraw();
				Time::EndFrame();
			},
				//OnSize
				[](Window* window) {
				Graphics::ResetCanvas();
				window->RePaint();
			});
		window.Show();

		//��ɳ�ʼ������ʽѭ��ǰ
		onStart();

		//��ʽѭ��
		HWND hwnd = window.GetHwnd();
		MSG msg = {};
		while (true)
		{
			//��֤��Ϣ���в�Ϊ�գ��Ӷ�һֱ���´���
			if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
				PostMessage(hwnd, WM_PAINT, NULL, NULL);

			//��ȡ��Ϣ
			GetMessage(&msg, hwnd, 0, 0);
			//Ԥ����󽻸����ڹ�����Ӧ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};