#include "GameObjectManager.h"
#include "GameObject.h"

namespace BDXKEngine
{
    GameObjectManager* GameObjectManager::Initialize(Window* window)
    {
        window->AddDestroyEvent([]
        {
            const std::vector allGameObjects = {GameObject::allGameObjects};
            for (const auto& value : allGameObjects)
                Object::Destroy(value.ToObjectBase());
            FlushDestroyQueue();
        });
        return nullptr;
    }
}
