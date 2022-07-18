#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine {
	std::vector<StartEvent*> Component::allStartEvents;
	std::vector<UpdateEvent*> Component::allUpdateEvents;
	std::vector<LateUpdateEvent*> Component::allLateUpdateEvents;

	ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
	ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }
}
