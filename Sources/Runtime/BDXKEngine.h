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

//�������ݽṹ
#include "Color.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
//����
#include "Math.h"
#include "String.h"
#include "Debug.h"
#include "WindowBase.h"
#include "Window.h"
//DirectX�ӿڷ�װ
#include "GL.h"
#include "GL2D.h"
//DirectX�ӿڷ�װ���ݽṹ
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
//ϵͳ
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
#include "Event.h"
#include "GUI.h"
//���
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"


class BDXKEngine :Time, Input, Screen, Cursor, Graphics, Event, GUI, GameObjectEditor, TransformEditor {
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
					Screen::Initialize(window);
					Input* input = Input::Initialize(window);
					Cursor* cursor = Cursor::Initialize(input,window);
					Event* event = Event::Initialize(input, window);
					::GL* gl = nullptr;
					::GL2D* gl2d = nullptr;
					Graphics* graphics = Graphics::Initialize(window, &gl, &gl2d);
					GUI::Initialize(gl2d, event, window);

					//��ɳ�ʼ������ʽѭ��ǰ
					onStart();
					break;
				}
				case WM_PAINT://֡����
				{
					GameObjectEditor::OnUpdate();
					ShowSceneInfo();
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

private:
	inline static std::wstring sceneInfo{ L"Hello BDXKEngine" };

	static void ShowSceneInfo()
	{
		float drawY = 10;
		std::vector<Transform*> rootTransforms = TransformEditor::GetRootTransforms();
		for (Transform* transform : rootTransforms)
		{
			drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
		}

		//��ʾ֡��
		GUI::TextArea({ 10,drawY,180,25 }, L"֡��:" + std::to_wstring(1 / Time::GetDeltaTime()));
		drawY += 30;

		//�ռ��޸��׽ڵ�
		{
			Rect orphanBox{ 10,drawY,100,25 };
			GUI::TextArea(orphanBox, L"�¶���");
			Transform* transform = nullptr;
			if (Event::IsDrop(orphanBox, (Component**)&transform))
			{
				if (transform != nullptr)
				{
					transform->SetParent(nullptr);
				}
			}
		}


		//��ʾ����
		Vector2 screenSize = Screen::GetSize();
		Rect rect{};
		rect.SetSize(screenSize / 4);
		rect.SetPosition({ screenSize.x - rect.width - 10, 10 });
		sceneInfo = GUI::TextArea(rect, sceneInfo);
	}
	static float ShowGameObjectInfo(GameObject* target, float drawY, int order = 0)
	{
		float drawX = (float)(10 + order * 30);
		//��ʾGameObject
		GUI::TextArea({ drawX,drawY,160,20 }, target->GetName());
		//���Transform�¼�
		Transform* transform = target->GetTransform();
		{
			Rect transformRect = { drawX + 165,drawY,160,15 };
			//ʹ�����ק
			if (Event::IsDrag(transformRect, transform))
			{
				::GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
			}
			//ʹ�������
			Transform* otherTransform = nullptr;
			if (Event::IsDrop(transformRect, (Component**)&otherTransform))
			{
				if (otherTransform != nullptr)
				{
					try
					{
						otherTransform->SetParent(transform);
						Event::Use();
					}
					catch (std::exception e)
					{
						Debug::Log(e);
					}
				}
			}
		}
		//��ʾComponents
		std::vector<Component*> components = target->GetComponents();
		for (Component* component : components)
		{
			drawX += 165;
			Rect rect = { drawX,drawY,160,15 };
			GUI::TextArea(rect, component->GetName(), 15);
			if (Event::IsFocus(rect))sceneInfo = component->ToString();
		}
		//��ʾ������
		if (transform->GetChildCount() != 0)
		{
			drawY += 25;
			for (int index = 0; index < transform->GetChildCount(); index++)
			{
				drawY = ShowGameObjectInfo(transform->GetChild(index)->GetGameObject(), drawY, order + 1);
			}
			return drawY;
		}

		return drawY + 25;
	}
};
