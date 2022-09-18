#include "GameObject.h"

namespace BDXKEngine {
	std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

	ObjectPtr<GameObject> GameObject::Create()
	{
		GameObject gameObject = {};

		return Object::Instantiate(&gameObject);
	}

	std::vector<ObjectPtr<Component>> GameObject::GetComponents()
	{
		return components;
	}
	ObjectPtr<Transform> GameObject::GetTransform()
	{
		return GetComponent<Transform>();
	}

	ObjectPtr<GameObject> GameObject::Find(std::wstring name)
	{
		return *std::find_if(allGameObjects.begin(), allGameObjects.end(), [=](ObjectPtr<GameObject> gameObject) {
			return gameObject->GetName() == name;
			});
	}

	void GameObject::Awake()
	{
		Object::Awake();

		AddComponent<Transform>();
		allGameObjects.push_back(this);
	}
	void GameObject::Destroy()
	{
		std::vector<ObjectPtr<Component>> components = this->components;
		for (ObjectPtr<Component>& component : components)
			Object::Destroy(component.GetObjectBase());

		allGameObjects.erase(std::find(
			allGameObjects.begin(),
			allGameObjects.end(),
			ObjectPtr<GameObject>{this}
		));

		Object::Destroy();
	}
}