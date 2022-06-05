#pragma once
#include<vector>
#include "Component.h"
#include "ComponentEvent.h"
#include "Transform.h"
#include "Object.h"
#include "String.h"

class Component;
class BDXKEngine;
class GameObject :public Object
{
	friend BDXKEngine;
public:
	GameObject(const wchar_t* name = L"New GameObject");

	Transform* GetTransform();

	template<typename TComponent>
	TComponent* AddComponent() {
		//确保TComponent实现相应的构造函数
		TComponent* component = new TComponent();
		component->gameObject = this;
		component->name = (String)typeid(TComponent).name();
		components.push_back(component);

		AwakeEvent* awakeEvent = dynamic_cast<AwakeEvent*>(component);
		if (awakeEvent != NULL)awakeEvent->Awake();

		StartEvent* startEvent = dynamic_cast<StartEvent*>(component);
		if (startEvent != NULL) startEvents.push_back(startEvent);

		UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(component);
		if (updateEvent != NULL) updateEvents.push_back(updateEvent);

		LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(component);
		if (lateUpdateEvent != NULL) lateUpdateEvents.push_back(lateUpdateEvent);

		OnRenderObjectEvent* onRenderObjectEvent = dynamic_cast<OnRenderObjectEvent*>(component);
		if (onRenderObjectEvent != NULL) onRenderObjectEvents.push_back(onRenderObjectEvent);

		OnDrawGizmosEvent* onDrawGizmosEvent = dynamic_cast<OnDrawGizmosEvent*>(component);
		if (onDrawGizmosEvent != NULL) onDrawGizmosEvents.push_back(onDrawGizmosEvent);

		return component;
	};
	template<typename TComponent>
	TComponent* GetComponent() {
		for (Component* component : components)
		{
			TComponent* target = dynamic_cast<TComponent*>(component);
			if (target != NULL)
				return target;
		}
		return NULL;
	}
private:
	static std::vector<GameObject*> gameObjects;//所有物体
	static std::vector<Component*> components;//所有组件
	//所有事件
	static std::vector<StartEvent*> startEvents;
	static std::vector<UpdateEvent*> updateEvents;
	static std::vector<LateUpdateEvent*> lateUpdateEvents;
	static std::vector<OnRenderObjectEvent*> onRenderObjectEvents;
	static std::vector<OnDrawGizmosEvent*> onDrawGizmosEvents;

	static void Update();

	Transform* transform;
};

