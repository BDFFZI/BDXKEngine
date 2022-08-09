#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine {
	Component::Component(std::wstring name) :Object(name)
	{
	}

	ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
	ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }

	void Component::OnAwake()
	{
		gameObject->components.push_back(this);
	}
	void Component::OnDestroy()
	{
		gameObject->components.erase(std::find(gameObject->components.begin(), gameObject->components.end(), ObjectPtr<Component>{this}));

		Object::OnDestroy();
	}
}
