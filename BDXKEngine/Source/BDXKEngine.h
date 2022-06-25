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
#include "Assert.h"
#include "WindowBase.h"
#include "Window.h"
//ϵͳ
#include "Graphics.h"
#include "Graphics2D.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
//���
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

class BDXKEngine :Time, Input, Screen, Cursor, Graphics, GameObjectEditor {
public:
	static void Run(std::function<void()> onStart)
	{
		std::setlocale(LC_ALL, "zh-CN");

		//��������
		Window window = { L"BDXKEngine",
			[&](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
				switch (messageSign)
				{
				case WM_CREATE://ϵͳ��ʼ��
				{
					Time::Initialize();
					Screen::Initialize(window);
					Cursor::Initialize(window);
					Graphics::Initialize(window->GetHwnd());
					Graphics2D::SetRenderTarget(window->GetHwnd());

					//��ɳ�ʼ������ʽѭ��ǰ
					onStart();
					return true;
				}
				case WM_PAINT://֡����
				{
					Time::BeginFrame();
					Graphics2D::BeginDraw();

					GameObjectEditor::OnUpdate();

					Graphics2D::EndDraw();
					Time::EndFrame();
					Input::FlushState();
					return true;
				}

#pragma region ����¼�
				case WM_MOUSEMOVE:
				{
					Input::mousePosition.x = (float)(lparameter << 48 >> 48);
					Input::mousePosition.y = (float)(lparameter >> 16);
					return true;
				}
				case WM_MOUSEWHEEL:
				{
					Input::mouseScrollDelta += GET_WHEEL_DELTA_WPARAM(wparameter) / 1000.0f;
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
#pragma region �����¼�
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
				case WM_SIZE:
				{
					Graphics2D::ResetCanvas();
					window->RePaint();
					return true;
				}
				case WM_SETCURSOR:
				{
					if (LOWORD(lparameter) == HTCLIENT)
					{
						Cursor::UpdateShow();
						return true;
					}
					return false;
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
				}
				return false;
			} };
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