#include "Component.h"
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/GameObject.h"


namespace BDXKEngine
{
    ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
    ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }


    void Component::Transfer(Transferrer& transferrer)
    {
        SwitchableObject::Transfer(transferrer);

        transferrer.TransferObjectPtr(nameof(gameObject), gameObject);
    }

    void Component::Awake()
    {
        ScriptableObject::Awake();

        if (IsActivatingAndEnabling())Enable();
    }

    void Component::Destroy()
    {
        if (IsActivatingAndEnabling())Disable();

        Vector::Remove(gameObject->components, {this});

        ScriptableObject::Destroy();
    }
    

    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling();
    }
}
