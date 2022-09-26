#include "GameObjectManager.h"
#include "GameObject.h"

namespace BDXKEngine
{
    GameObjectManager* GameObjectManager::Initialize(Window* window)
    {
        window->AddDestroyEvent([]
        {
            for (const auto& value : GameObject::allGameObjects)
                Object::DestroyImmediate(value.ToObjectBase());
        });
        return nullptr;
    }
}
