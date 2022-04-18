#pragma once
#include "Component.h"
#include "ComponentEvent.h"
#include "String.h"
#include "Transform.h"
#include "List.h"
#include "Object.h"
#include "Debug.h"

class Component;
class BDXKEngine;
class GameObject :public Object
{
	friend BDXKEngine;
public:
	GameObject(String name = String(L""));

	Transform* GetTransform();

	template<typename TComponent>
	TComponent* AddComponent() {
		//ȷ��TComponentʵ����Ӧ�Ĺ��캯��
		TComponent* component = new TComponent();
		component->gameObject = this;
		component->name = name + " : " + typeid(TComponent).name();
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
	static List<GameObject*> gameObjects;//��������
	static List<Component*> components;//�������
	//�����¼�
	static List<StartEvent*> startEvents;
	static List<UpdateEvent*> updateEvents;
	static List<LateUpdateEvent*> lateUpdateEvents;
	static List<OnRenderObjectEvent*> onRenderObjectEvents;
	static List<OnDrawGizmosEvent*> onDrawGizmosEvents;

	static void Update();

	Transform* transform;
};

