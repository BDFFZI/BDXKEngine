#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine {
	std::vector<StartEvent*> Component::allStartEvents;
	std::vector<UpdateEvent*> Component::allUpdateEvents;
	std::vector<LateUpdateEvent*> Component::allLateUpdateEvents;

	Component::Component(std::wstring name) :Object(name)
	{
	}

	ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
	ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }

	void Component::OnAwake()
	{
		gameObject->components.push_back(this);

		StartEvent* startEvent = dynamic_cast<StartEvent*>(this);
		if (startEvent != nullptr) allStartEvents.push_back(startEvent);

		UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(this);
		if (updateEvent != nullptr) allUpdateEvents.push_back(updateEvent);

		LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(this);
		if (lateUpdateEvent != nullptr) allLateUpdateEvents.push_back(lateUpdateEvent);
	}
	void Component::OnDestroy()
	{
		StartEvent* startEvent = dynamic_cast<StartEvent*>(this);
		if (startEvent != nullptr) {
			auto pos = std::find(allStartEvents.begin(), allStartEvents.end(), startEvent);
			if (pos != allStartEvents.end())allStartEvents.erase(pos);
		}

		UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(this);
		if (updateEvent != nullptr) allUpdateEvents.erase(std::find(allUpdateEvents.begin(), allUpdateEvents.end(), updateEvent));

		LateUpdateEvent* lateUpdateEvent = dynamic_cast<LateUpdateEvent*>(this);
		if (lateUpdateEvent != nullptr) allLateUpdateEvents.erase(std::find(allLateUpdateEvents.begin(), allLateUpdateEvents.end(), lateUpdateEvent));

		gameObject->components.erase(std::find(gameObject->components.begin(), gameObject->components.end(), ObjectPtr<Component>{this}));

		Object::OnDestroy();
	}
}
