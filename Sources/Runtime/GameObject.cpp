#include "GameObject.h"

namespace BDXKEngine {
	std::vector<GameObject*> GameObject::gameObjects;
	std::vector<Component*> GameObject::components;
	std::vector<StartEvent*> GameObject::startEvents;
	std::vector<UpdateEvent*> GameObject::updateEvents;
	std::vector<LateUpdateEvent*> GameObject::lateUpdateEvents;

	GameObject* GameObject::Find(std::wstring name)
	{
		return *std::find_if(gameObjects.begin(), gameObjects.end(), [=](GameObject* gameObject) {
			return gameObject->GetName() == name;
			});
	}

	GameObject::GameObject(std::wstring name) {
		SetName(name);
		AddComponent<Transform>();
		gameObjects.push_back(this);
	}

	std::vector<Component*> GameObject::GetComponents()
	{
		return ownedComponents;
	}

	Transform* GameObject::GetTransform()
	{
		return GetComponent<Transform>();
	}

	void GameObjectEditor::OnUpdate()
	{
		for (StartEvent* eventP : GameObject::startEvents)
			TickEventEditor::Start(eventP);
		GameObject::startEvents.clear();

		for (UpdateEvent* eventP : GameObject::updateEvents)
			TickEventEditor::Update(eventP);

		for (LateUpdateEvent* eventP : GameObject::lateUpdateEvents)
			TickEventEditor::LateUpdate(eventP);
	}
}