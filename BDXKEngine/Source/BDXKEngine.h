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

//�������ݽṹ
#include <functional>
#include "String.h"
#include "Com.h"
#include "Color.h"
#include "Vector2.h"
#include "Math.h"
#include "Time.h"
#include "Matrix.h"
#include "Input.h"
//��չ����
#include "WindowBase.h"
#include "Window.h"
#include "Debug.h"
#include "Assert.h"
#include "GameObject.h"
#include "Component.h"
#include "Screen.h"
#include "Cursor.h"

class BDXKEngine {
public:
	static void Run(std::function<void()> onStart)
	{
		std::setlocale(LC_ALL, "zh-CN");
		//��ʼ��
		Window window = Window(L"BDXKEngine",
			[](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
				switch (messageSign)
				{
				case WM_CREATE:
				{
					Graphics::SetRenderTarget(window->GetHwnd());
					Screen::Initialize(window);
					Time::Initialize();
					return true;
				}
				case WM_CLOSE:
				{
					if (MessageBox(window->GetHwnd(), L"ȷ���رգ�", L"�رմ���", MB_OKCANCEL) == IDOK)
						DestroyWindow(window->GetHwnd());
					return true;
				}
				case WM_DESTROY:
				{
					PostQuitMessage(0);
					return true;
				}
				case WM_PAINT:
				{
					Time::BeginFrame();
					Graphics::BeginDraw();

					Graphics::ClearCanvas();

					for (GameObject* gameObject : GameObject::gameObjects)
					{
						gameObject->Update();
					}

					Graphics::EndDraw();
					Time::EndFrame();
					Input::Update();
					return true;
				}
				case WM_SIZE:
				{
					Graphics::ResetCanvas();
					window->RePaint();
					return true;
				}
#pragma region ���
				case WM_MOUSEMOVE:
				{
					Input::mousePosition.x = (float)(lparameter << 48 >> 48);
					Input::mousePosition.y = (float)(lparameter >> 16);
					return true;
				}
				case WM_MOUSEWHEEL:
				{
					Input::mouseScrollDelta.y += GET_WHEEL_DELTA_WPARAM(wparameter) / 1000.0f;
					return true;
				}
				case WM_LBUTTONDOWN:
				{
					Input::mouseButtonState[0] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_LBUTTONUP:
				{
					Input::mouseButtonState[0] = false;
					ReleaseCapture();
					return true;
				}
				case WM_RBUTTONDOWN:
				{
					Input::mouseButtonState[1] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_RBUTTONUP:
				{
					Input::mouseButtonState[1] = false;
					ReleaseCapture();
					return true;
				}
				case WM_MBUTTONDOWN:
				{
					Input::mouseButtonState[2] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_MBUTTONUP:
				{
					Input::mouseButtonState[2] = false;
					ReleaseCapture();
					return true;
				}
#pragma endregion
				}
				return false;
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