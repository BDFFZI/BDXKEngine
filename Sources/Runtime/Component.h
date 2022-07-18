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

	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
		friend GameObjectEditor;
	public:
		//设为虚函数是为了转为多态类型
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
		//所有更新事件
		static std::vector<StartEvent*> allStartEvents;//由Component和GameObjectEditor控制
		static std::vector<UpdateEvent*> allUpdateEvents;//由Component控制
		static std::vector<LateUpdateEvent*> allLateUpdateEvents;//由Component控制

		ObjectPtr<GameObject> gameObject = 0;
	};
}
