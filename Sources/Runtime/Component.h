#pragma once
#include<algorithm>
#include "ObjectPtr.h"

namespace BDXKEngine {
	class GameObjectManager;
	class GameObject;
	class Transform;

	class StartEvent {
		friend GameObjectManager;
	protected:
		virtual void OnStart() = 0;
	};
	class UpdateEvent {
		friend GameObjectManager;
	protected:
		virtual void OnUpdate() = 0;
	};
	class LateUpdateEvent {
		friend GameObjectManager;
	protected:
		virtual void OnLateUpdate() = 0;
	};

	//ȷ��TComponent��Ĭ�Ϲ��캯��
	class Component :public Object
	{
		friend GameObject;
		friend GameObjectManager;
	public:
		Component(std::wstring name = L"New Component");

		//��Ϊ�麯����Ϊ��תΪ��̬����
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		//��ȫ����֮����дʱ����ػص�
		virtual void OnAwake();
		//��ȫɾ��֮ǰ����дʱ����ػص�
		void OnDestroy()override;
	private:
		//���и����¼�
		static std::vector<StartEvent*> allStartEvents;//��Component��GameObjectEditor����
		static std::vector<UpdateEvent*> allUpdateEvents;//��Component����
		static std::vector<LateUpdateEvent*> allLateUpdateEvents;//��Component����

		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
