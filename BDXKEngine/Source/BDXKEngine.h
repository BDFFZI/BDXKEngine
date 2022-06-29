#pragma once
//C++��������ʾ�����������ʹ��
//W ����wchar_t,�Դ���Unicode��utf-16,ע�����ڶ������
//L ����x16ʱ���Ĳ�����ڲ���Ҳ�ǳ�
//P ָ��
//H ���
//LP ��ָ��
//C ����

//C++���⺯����׺���������Ĳ�ͬ�汾
// 
//������ʹ�õ��ַ�����
//a ASCII����
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

//����
#include "Color.h"
#include "Rect.h"
#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
#include "Mesh.h"
//����
#include "String.h"
#include "Debug.h"
#include "WindowBase.h"
#include "Window.h"
//ϵͳ
#include "Graphics.h"
#include "GL2D.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
#include "GUI.h"
//���
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"


class BDXKEngine :Time, Input, Screen, Cursor, Graphics, GUI, GameObjectEditor {
public:
	static void Run(std::function<void()> onStart)
	{
		std::setlocale(LC_ALL, "zh-CN");

		//��������
		Window window = {
			L"BDXKEngine",
			[&](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
				switch (messageSign)
				{
				case WM_CREATE://ϵͳ��ʼ��
				{
					Time::Initialize(window);
					Input* input = Input::Initialize(window);
					Screen::Initialize(window);
					Cursor* cursor = Cursor::Initialize(input,window);
					::GL* gl = nullptr;
					::GL2D* gl2d = nullptr;
					Graphics* graphics = Graphics::Initialize(window, &gl, &gl2d);
					GUI::Initialize(gl2d, input, window);

					//��ɳ�ʼ������ʽѭ��ǰ
					onStart();
					break;
				}
				case WM_PAINT://֡����
				{
					GameObjectEditor::OnUpdate();
					break;
				}
				//case WM_CLOSE:
				//{
				//	if (MessageBox(window->GetHwnd(), L"ȷ���رգ�", L"�رմ���", MB_OKCANCEL) == IDOK)
				//		DestroyWindow(window->GetHwnd());
				//	return LRESULT{0};
				//}
				//case WM_DESTROY:
				//{
				//	PostQuitMessage(0);
				//	break;
				//}
				}

				return DefWindowProcW(window->GetHwnd(), messageSign, wparameter, lparameter);
			}
		};
		window.Show();

		//��ʽѭ��
		HWND hwnd = window.GetHwnd();
		MSG msg = {};
		while (true)
		{
			//���û��Ҫ�������Ϣ�����Ǿ�����ο���ʱ�������Ϸ
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
