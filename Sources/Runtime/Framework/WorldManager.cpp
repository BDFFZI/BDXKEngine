#include "WorldManager.h"

namespace BDXKEngine {
	WorldManager* WorldManager::Initialize(Window* window)
	{
		window->AddDestroyEvent([]() {
			for (ObjectPtr<GameObject> gameObject : std::vector<ObjectPtr<GameObject>>{ GameObject::allGameObjects })
				Object::DestroyImmediate(gameObject.ToObjectBase());
			});
		return nullptr;
	}
}

