#pragma once
#include<vector>
#include<map>
#include "TickEvent.h"
#include "String.h"
#include "ObjectPtr.h"
#include "Component.h"
#include "Transform.h"

namespace BDXKEngine {
	class GameObjectEditor;
	class GameObject :public Object, ObjectEditor, TickEventEditor
	{
		friend GameObjectEditor;
	public:
		static ObjectPtr<GameObject> Find(std::wstring name);

		GameObject(std::wstring name = L"New GameObject");

		template<typename TComponent>
		ObjectPtr<TComponent> AddComponent() {
			//初始化Component
			ObjectPtr<TComponent> component{ new TComponent() };
			component->gameObject = this;
			component->SetName((String)typeid(TComponent).name());
			//注册组件
			components.push_back({ component });
			//注册事件
			AwakeEvent* awakeEvent = dynamic_cast<AwakeEvent*>(component.GetPtr());
			if (awakeEvent != nullptr) TickEventEditor::Awake(awakeEvent);
			StartEvent* startEvent = dynamic_cast<StartEvent*>(component.GetPtr());
			if (startEvent != nullptr) allStartEvents.push_back(startEvent);
			UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(component.GetPtr());
			if (updateEvent != nullptr) allUpdateEvents.push_back(updateEvent);
			LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(component.GetPtr());
			if (lateUpdateEvent != nullptr) allLateUpdateEvents.push_back(lateUpdateEvent);

			return component;
		};

		template<typename TComponent>
		ObjectPtr<TComponent> GetComponent() {
			for (ObjectPtr<Component>& component : components)
			{
				ObjectPtr<TComponent> target = component.As<TComponent>();
				if (target != nullptr)
					return target;
			}
			return nullptr;
		}
		std::vector<ObjectPtr<Component>> GetComponents();
		ObjectPtr<Transform> GetTransform();

	private:
		//所有物体
		static std::vector<ObjectPtr<GameObject>> allGameObjects;
		//所有事件
		static std::vector<StartEvent*> allStartEvents;
		static std::vector<UpdateEvent*> allUpdateEvents;
		static std::vector<LateUpdateEvent*> allLateUpdateEvents;

		//当前物体拥有的组件
		std::vector<ObjectPtr<Component>> components;
	};

	class GameObjectEditor {
	protected:
		static void Update();
		static void Release();
	};
}