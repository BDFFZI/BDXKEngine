#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine {
	GameObject* Component::GetGameObject() { return gameObject; }
	Transform* Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }
}
