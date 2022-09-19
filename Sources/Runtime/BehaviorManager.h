#pragma once
#include <unordered_map>
#include "Window.h"
#include "BehaviorEvent.h"
#include "Debug.h"

namespace BDXKEngine {
	class Behavior;
	class BehaviorManager
	{
		friend Behavior;
	public:
		static void DebugHandlersCount()
		{
			std::wstringstream stream = {};
			stream << L"StartHandler����:" << std::to_wstring(allStartHandlers.size()) << std::endl;
			stream << L"UpdateHandler����:" << std::to_wstring(allUpdateHandlers.size()) << std::endl;
			stream << L"LateUpdateHandler����:" << std::to_wstring(allLateUpdateHandlers.size());
			Debug::LogWarning(stream.str());
		}

		static BehaviorManager* Initialize(Window* window);
	private:
		static void Start();
		static void Update();
		static void LateUpdate();

		//����ɾ������Ĺ��ܣ��ᵼ�±�������ʱ����Ұָ��������Ϊ��֤���������������У���Ҫ�ӳٴ���
		//ͨ��invalid��Ǹ�֪BehaviorManager���Ա�ȵ���ȫ״̬ʱ��ͳһ����
		//���ʱҲ�ܱ��ӳٴ���
		static std::unordered_map<StartHandler*, bool> allStartHandlers;
		static std::unordered_map<UpdateHandler*, bool> allUpdateHandlers;
		static std::unordered_map<LateUpdateHandler*, bool> allLateUpdateHandlers;
	};

}

