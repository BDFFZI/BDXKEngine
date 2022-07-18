#include "GameObject.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

	ObjectPtr<GameObject> GameObject::Find(std::wstring name)
	{
		return *std::find_if(allGameObjects.begin(), allGameObjects.end(), [=](ObjectPtr<GameObject> gameObject) {
			return gameObject->GetName() == name;
			});
	}

	GameObject::GameObject(std::wstring name) {
		SetName(name);
		AddComponent<Transform>();
		allGameObjects.push_back(this);
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
		for (StartEvent* eventP : Component::allStartEvents)
			eventP->OnStart();
		Component::allStartEvents.clear();

		for (UpdateEvent* eventP : Component::allUpdateEvents)
			eventP->OnUpdate();

		for (LateUpdateEvent* eventP : Component::allLateUpdateEvents)
			eventP->OnLateUpdate();
	}
	void GameObjectEditor::Release()
	{
		std::vector<ObjectPtr<GameObject>> allGameObjects = GameObject::allGameObjects;
		for (auto gameObject : allGameObjects)
			Object::Destroy(gameObject);
		allGameObjects.clear();
	}
}