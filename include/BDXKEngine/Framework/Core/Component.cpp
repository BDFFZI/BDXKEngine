#include "Component.h"

namespace BDXKEngine
{
    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling() && ScriptableObject::GetIsActivating();
    }
    ObjectPtr<GameObject> Component::GetGameObject() const { return gameObject; }

    void Component::Transfer(Transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        TransferFieldInfo(gameObject);
    }
    void Component::Destroy()
    {
        gameObject->RemoveComponent(this);

        ScriptableObject::Destroy();
    }
}
