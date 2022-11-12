#include "Component.h"
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/GameObject/GameObject.h"
#include "BDXKEngine/Framework/Component/Transform.h"

namespace BDXKEngine
{
    ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
    ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }
    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling();
    }
    
    void Component::Transfer(transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        if (transferer.GetTransferDirection() != TransferDirection::Inspect)
            transferer.TransferField(nameof(gameObject), gameObject);
    }
    void Component::Destroy()
    {
        Vector::Remove(gameObject->components, {this});
        
        ScriptableObject::Destroy();
    }
}
