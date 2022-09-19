#pragma once
#include <unordered_set>
#include "Window.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	class Behavior;
	class BehaviorManager
	{
		friend Behavior;
	public:
		static BehaviorManager* Initialize(Window* window);
	private:
		static void Start();
		static void Update();
		static void LateUpdate();

		static std::vector<StartHandler*> allStartHandlers;
		static std::vector<UpdateHandler*> allUpdateHandlers;
		static std::vector<LateUpdateHandler*> allLateUpdateHandlers;
		//����ɾ������Ĺ��ܣ��ᵼ�±�������ʱ����Ԫ����Ч�������δ��֤���������������У���Ҫ�ӳٴ���
		//ͨ��invalid��Ǹ�֪BehaviorManager���Ա�ȵ���ȫ״̬ʱ��ͳһ����
		static std::unordered_set<StartHandler*> invalidStartHandlers;
		static std::unordered_set<UpdateHandler*> invalidUpdateHandlers;
		static std::unordered_set<LateUpdateHandler*> invalidLateUpdateHandlers;
	};

}

