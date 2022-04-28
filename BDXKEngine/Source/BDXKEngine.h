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
#include<algorithm>
#include "Com.h"
#include "String.h"
#include "Object.h"
#include "List.h"
#include "Color.h"
#include "Rect.h"
#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
#include "Mesh.h"
//����
#include "Debug.h"
#include "Assert.h"
#include "WindowBase.h"
#include "Window.h"
//ϵͳ
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
//���
#include "GameObject.h"
#include "Component.h"
#include "ComponentEvent.h"
#include "Transform.h"
#include "Renderer.h"

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
				case WM_PAINT:
				{
					Time::BeginFrame();
					Graphics::BeginDraw();
					Graphics::ClearCanvas();

					////ͳ�ƴ����µ����壬�Բ�νṹΪ���ȼ�
					//List<MatrixBuffer*> list;
					//list.push_back(&MatrixBuffer::root);
					//for (int i = 0; i < list.size(); i++)
					//{
					//	for (MatrixBuffer* child : list[i]->children)
					//	{
					//		list.push_back(child);
					//	}
					//}
					////�������壬����һ��Ϊϵͳ�����壬������
					//std::for_each(
					//	(list.begin() + 1), list.end(),
					//	[](MatrixBuffer* item) {
					//		item->GetGameObject()->Update();
					//	}
					//);
					GameObject::Update();

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
				case WM_SETCURSOR:
				{
					if (LOWORD(lparameter) == HTCLIENT)
					{
						Cursor::Update();
						return true;
					}
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
					if (Input::GetMouseButton(1) == false &&
						Input::GetMouseButton(2) == false)
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
					if (Input::GetMouseButton(0) == false &&
						Input::GetMouseButton(2) == false)
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
					if (Input::GetMouseButton(0) == false &&
						Input::GetMouseButton(1) == false)
						ReleaseCapture();
					return true;
				}
#pragma endregion
#pragma region ����
				case WM_KEYDOWN:
				{
					Input::keyboardState[wparameter] = true;
					return true;
				}
				case WM_KEYUP:
				{
					Input::keyboardState[wparameter] = false;
					return true;
				}
#pragma endregion
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