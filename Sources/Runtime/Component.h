#pragma once
#include<algorithm>
#include "ObjectPtr.h"

namespace BDXKEngine {
	class GameObjectEditor;
	class GameObject;
	class Transform;

	class StartEvent {
		friend GameObjectEditor;
	protected:
		virtual void OnStart() = 0;
	};
	class UpdateEvent {
		friend GameObjectEditor;
	protected:
		virtual void OnUpdate() = 0;
	};
	class LateUpdateEvent {
		friend GameObjectEditor;
	protected:
		virtual void OnLateUpdate() = 0;
	};

	//ȷ��TComponent��Ĭ�Ϲ��캯��
	class Component :public Object
	{
		friend GameObject;
		friend GameObjectEditor;
	public:
		//��Ϊ�麯����Ϊ��תΪ��̬����
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		virtual void OnAwake()
		{
			StartEvent* startEvent = dynamic_cast<StartEvent*>(this);
			if (startEvent != nullptr) allStartEvents.push_back(startEvent);

			UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(this);
			if (updateEvent != nullptr) allUpdateEvents.push_back(updateEvent);

			LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(this);
			if (lateUpdateEvent != nullptr) allLateUpdateEvents.push_back(lateUpdateEvent);
		}

		void OnDestroy()override {

			StartEvent* startEvent = dynamic_cast<StartEvent*>(this);
			if (startEvent != nullptr) {
				auto pos = std::find(allStartEvents.begin(), allStartEvents.end(), startEvent);
				if (pos != allStartEvents.end())allStartEvents.erase(pos);
			}

			UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(this);
			if (updateEvent != nullptr) allUpdateEvents.erase(std::find(allUpdateEvents.begin(), allUpdateEvents.end(), updateEvent));

			LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(this);
			if (lateUpdateEvent != nullptr) allLateUpdateEvents.erase(std::find(allLateUpdateEvents.begin(), allLateUpdateEvents.end(), lateUpdateEvent));

			Object::OnDestroy();
		}
	private:
		//���и����¼�
		static std::vector<StartEvent*> allStartEvents;//��Component��GameObjectEditor����
		static std::vector<UpdateEvent*> allUpdateEvents;//��Component����
		static std::vector<LateUpdateEvent*> allLateUpdateEvents;//��Component����

		ObjectPtr<GameObject> gameObject = 0;
	};
}
