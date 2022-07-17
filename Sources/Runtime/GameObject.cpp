#include "GameObject.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;
	std::vector<StartEvent*> GameObject::allStartEvents;
	std::vector<UpdateEvent*> GameObject::allUpdateEvents;
	std::vector<LateUpdateEvent*> GameObject::allLateUpdateEvents;

	ObjectPtr<GameObject> GameObject::Find(std::wstring name)
	{
		return *std::find_if(allGameObjects.begin(), allGameObjects.end(), [=](ObjectPtr<GameObject> gameObject) {
			return gameObject->GetName() == name;
			});
	}

	GameObject::GameObject(std::wstring name) {
		SetName(name);
		AddComponent<Transform>();
		allGameObjects.push_back(ObjectPtr<GameObject>{this});
	}

	std::vector<ObjectPtr<Component>> GameObject::GetComponents()
	{
		return components;
	}

	ObjectPtr<Transform> GameObject::GetTransform()
	{
		return GetComponent<Transform>();
	}

	void GameObjectEditor::Update()
	{
		for (StartEvent* eventP : GameObject::allStartEvents)
			TickEventEditor::Start(eventP);
		GameObject::allStartEvents.clear();

		for (UpdateEvent* eventP : GameObject::allUpdateEvents)
			TickEventEditor::Update(eventP);

		for (LateUpdateEvent* eventP : GameObject::allLateUpdateEvents)
			TickEventEditor::LateUpdate(eventP);
	}
	void GameObjectEditor::Release()
	{
		for (ObjectPtr<GameObject>& gameObject : GameObject::allGameObjects)
		{
			Object::Destroy(gameObject);
		}
		GameObject::allGameObjects.clear();
	}
}