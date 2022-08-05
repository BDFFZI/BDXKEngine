#include "GameObjectManager.h"
using namespace BDXKEngine;

GameObjectManager* GameObjectManager::Initialize(Window* window)
{
	window->AddMessageListener(OnWindowMessage);
	return new GameObjectManager{};
}

void GameObjectManager::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign) {
	case WM_PAINT:
	{
		for (StartEvent*& eventP : std::vector<StartEvent*>{ Component::allStartEvents })
			eventP->OnStart();
		Component::allStartEvents.clear();

		for (UpdateEvent*& eventP : std::vector<UpdateEvent*>{ Component::allUpdateEvents })
			eventP->OnUpdate();

		for (LateUpdateEvent*& eventP : std::vector<LateUpdateEvent*>{ Component::allLateUpdateEvents })
			eventP->OnLateUpdate();
		break;
	}
	case WM_DESTROY:
	{
		for (ObjectPtr<GameObject> gameObject : std::vector<ObjectPtr<GameObject>>{ GameObject::allGameObjects })
			Object::DestroyImmediate(gameObject.GetPtr());
		break;
	}
	}
}
