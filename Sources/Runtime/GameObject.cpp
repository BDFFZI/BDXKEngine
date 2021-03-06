#include "GameObject.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

	ObjectPtr<GameObject> GameObject::Find(std::wstring name)
	{
		return *std::find_if(allGameObjects.begin(), allGameObjects.end(), [=](ObjectPtr<GameObject> gameObject) {
			return gameObject->GetName() == name;
			});
	}

	GameObject::GameObject(std::wstring name) :Object(name) {
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

	void GameObject::OnDestroy()
	{
		std::vector<ObjectPtr<Component>> components = this->components;
		for (ObjectPtr<Component>& component : components)
			Destroy(component.GetPtr());

		allGameObjects.erase(std::find(
			allGameObjects.begin(),
			allGameObjects.end(),
			ObjectPtr<GameObject>{this}
		));

		Object::OnDestroy();
	}
}