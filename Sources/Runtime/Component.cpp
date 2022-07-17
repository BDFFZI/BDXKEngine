#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine {
	ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
	ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }
}
