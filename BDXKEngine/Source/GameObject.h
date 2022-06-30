#pragma once
#include<vector>
#include<map>
#include "TickEvent.h"
#include "String.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"

class GameObjectEditor;
class GameObject :public Object, ObjectEditor, TickEventEditor
{
	friend GameObjectEditor;
public:
	GameObject(const wchar_t* name = L"New GameObject");

	template<typename TComponent>
	TComponent* AddComponent() {
		//��ʼ��Component
		TComponent* component = new TComponent();
		component->gameObject = this;
		ObjectEditor::InitializeObject(component, this->GetID() + (int)ownedComponents.size(), (String)typeid(TComponent).name());
		//ע�����
		components.push_back(component);
		ownedComponents.push_back(component);
		//ע���¼�
		AwakeEvent* awakeEvent = dynamic_cast<AwakeEvent*>(component);
		if (awakeEvent != NULL) TickEventEditor::Awake(awakeEvent);
		StartEvent* startEvent = dynamic_cast<StartEvent*>(component);
		if (startEvent != NULL) startEvents.push_back(startEvent);
		UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(component);
		if (updateEvent != NULL) updateEvents.push_back(updateEvent);
		LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(component);
		if (lateUpdateEvent != NULL) lateUpdateEvents.push_back(lateUpdateEvent);
		RenderObjectEvent* renderObjectEvent = dynamic_cast<RenderObjectEvent*>(component);
		if (renderObjectEvent != NULL) renderObjectEvents.push_back(renderObjectEvent);
		DrawGizmosEvent* drawGizmosEvent = dynamic_cast<DrawGizmosEvent*>(component);
		if (drawGizmosEvent != NULL) drawGizmosEvents.push_back(drawGizmosEvent);

		return component;
	};
	template<typename TComponent>
	TComponent* GetComponent() {
		for (Component* component : ownedComponents)
		{
			TComponent* target = dynamic_cast<TComponent*>(component);
			if (target != NULL)
				return target;
		}
		return NULL;
	}
	std::vector<Component*> GetComponents();
	Transform* GetTransform();
private:
	static std::vector<GameObject*> gameObjects;//��������
	static std::vector<Component*> components;//�������
	//�����¼�
	static std::vector<StartEvent*> startEvents;
	static std::vector<UpdateEvent*> updateEvents;
	static std::vector<LateUpdateEvent*> lateUpdateEvents;
	static std::vector<RenderObjectEvent*> renderObjectEvents;
	static std::vector<DrawGizmosEvent*> drawGizmosEvents;

	std::vector<Component*> ownedComponents;//��ǰ����ӵ�е�
};

class GameObjectEditor {
protected:
	static void OnUpdate();
};