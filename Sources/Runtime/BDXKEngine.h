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

//�����㣺�����������ͣ�ͳһ��ģ�齻�������� >> ��չ��׼��
#include "ObjectPtr.h"
#include "Color.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
//ƽ̨�㣺�Բ���ϵͳ�Լ����ַǱ�׼��ķ�װ >> ��չ��׼��
#include "String.h"
#include "Debug.h"
#include "WindowBase.h"
#include "Window.h"
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
//��Դ�㣺�Ը����ⲿ�ļ���������־û���Դ�ı�ʾ�Ͷ�д >> �������������е�����Դͷ��δ���༭���ı༭Ŀ��
#include "Resources.h"
#include "MeshImporter.h"
//���ܲ㣺�����������ṩ�ĸ�������ʱ���� >> �����������������
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
#include "Event.h"
#include "GUI.h"
#include "Random.h"
//��ܲ㣺�ڴ���֯��ʹ���²�ĸ��ֹ��� >> �û���������Ľӿ�
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"

#define GetResourcesPath(Type,Name) "../Resources/"#Type"/"#Name""
#define GetResourcesPathW(Type,Name) L"../Resources/"#Type"/"#Name""

namespace BDXKEngine {
	class Engine :Time, Input, Screen, Cursor, Graphics, Event, GUI, GameObjectEditor, TransformEditor {
	public:
		static void Run(::std::function<void()> onStart)
		{
			std::setlocale(LC_ALL, "zh-CN");

			//��������
			Window window = {
				L"BDXKEngine",
				[&](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
					switch (messageSign)
					{
					case WM_CREATE:
					{
						//���ܳ�ʼ��
						Time::Initialize(window);
						Screen::Initialize(window);
						Input* input = Input::Initialize(window);
						Cursor* cursor = Cursor::Initialize(input,window);
						Event* event = Event::Initialize(input, window);
						GL* gl = nullptr;
						GL2D* gl2d = nullptr;
						Graphics* graphics = Graphics::Initialize(window, &gl, &gl2d);
						GUI::Initialize(gl2d, event, window);

						//����������Ϣ���⽫Ӱ���ܲ��в���ģ��ʹ�ù��ܲ�ķ�ʽ
						CreateSettings();

						//��ɳ�ʼ������ʽѭ��ǰ
						onStart();
						break;
					}
					case WM_PAINT:
					{
						//֡����
						GameObjectEditor::Update();
						break;
					}
					case WM_DESTROY:
					{
						GameObjectEditor::Release();
						ReleaseSettings();
						break;
					}
					//case WM_CLOSE:
					//{
					//	if (MessageBox(window->GetHwnd(), L"ȷ���رգ�", L"�رմ���", MB_OKCANCEL) == IDOK)
					//		DestroyWindow(window->GetHwnd());
					//	return LRESULT{0};
					//}

					}

					return DefWindowProcW(window->GetHwnd(), messageSign, wparameter, lparameter);
				}
			};
			window.Show();

			//��ʽѭ��
			HWND hwnd = window.GetHwnd();
			MSG msg = {};
			while (GetMessage(&msg, hwnd, 0, 0) > 0)
			{
				//Ԥ����󽻸����ڹ�����Ӧ
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//���û��Ҫ�������Ϣ�����Ǿ�����ο���ʱ�������Ϸ
				if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
					PostMessage(hwnd, WM_PAINT, NULL, NULL);
			}
		}
	private:
		static void CreateSettings() {
			GraphicsSettings::shadowShader = new Shader(
				GetResourcesPathW(Shaders, ShadowMap\\VertexShader.hlsl),
				GetResourcesPathW(Shaders, ShadowMap\\PixelShader.hlsl),
				PassType::ForwardBase
			);
		}
		static void ReleaseSettings()
		{
			Object::Destroy(GraphicsSettings::shadowShader.GetPtr());
		}
	};
}
