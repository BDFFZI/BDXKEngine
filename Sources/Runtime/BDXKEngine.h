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
#include "TextureCube.h"
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
#include "RendererManager.h"
#include "BehaviorManager.h"
#include "WorldManager.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"



namespace BDXKEngine {
	class Engine :Resources, Time, Input, Screen, Cursor, Event, Graphics, GUI, TransformEditor, WorldManager {
	public:
		static void Run(std::function<void()> onStart)
		{
			std::setlocale(LC_ALL, "zh-CN");

			//��������
			Window window = { L"BDXKEngine" };

			//ƽ̨���ʼ��
			GL::Initialize(&window);
			GL2D::Initialize();
			//��Դ���ʼ��
			Resources::Initialize(&window);
			//���������Ǿ�̬�ģ�˳���ʼ�����Ա��ʼ�����쳣

			//����������Ϣ���⽫Ӱ���ܲ��в���ģ���������ʽ
			CreateSettings();

			//���������Ƕ�̬�ģ������ʼ������Ϊ��Ӱ���¼�˳�򣩣��Ա�ʹ�û������쳣
			BehaviorManager::Initialize(&window);
			RendererManager::Initialize(&window);
			WorldManager::Initialize(&window);
			GUI::Initialize(&window);
			Event::Initialize(&window);
			Graphics::Initialize(&window);
			Screen::Initialize(&window);
			Cursor::Initialize(&window);
			Input::Initialize(&window);
			Time::Initialize(&window);

			//��ɳ�ʼ������ʽѭ��ǰ�������¼��ص�
			onStart();

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

			ReleaseSettings();

			Debug::LogError(L"ϵͳ���ռ��");
			ObjectPtrBase::DebugRefCountMap();
			BehaviorManager::DebugHandlersCount();
		}
	private:
		static void CreateSettings()
		{

		}

		static void ReleaseSettings() {
			GraphicsSettings::skybox = nullptr;
		}
	};
}
