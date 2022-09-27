#include "Component.h"
#include "GameObject.h"
#include "Base/Extension/Vector.h"


namespace BDXKEngine
{
    ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
    ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }


    void Component::Export(Exporter& exporter)
    {
        Object::Export(exporter);

        exporter.TransferObjectPtr({}, gameObject);
        exporter.TransferBool({}, isEnabling);
    }

    void Component::Import(Importer& importer)
    {
        Object::Import(importer);

        gameObject = importer.TransferObjectPtr({});
        isEnabling = importer.TransferBool({});
    }

    void Component::Awake()
    {
        Object::Awake();
        
        if (IsActivatingAndEnabling())Enable();
    }

    void Component::Destroy()
    {
        if (IsActivatingAndEnabling())Disable();
        
        Vector::Remove(gameObject->components, {this});
        
        Object::Destroy();
    }

    void Component::Enable()
    {
        //Debug::LogWarning(static_cast<String>(L"Component::Enable ") + GetInstanceID() + " " + GetName());
    }

    void Component::Disable()
    {
        //Debug::LogWarning(static_cast<String>(L"Component::Disable ") + GetInstanceID() + " " + GetName());
    }
    
    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling();
    }
    void Component::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;

        if (GetIsActivating())
        {
            if (GetIsRunning())
            {
                if (state)
                    Enable();
                else
                    Disable();
            }
        }

        isEnabling = state;
    }
    void Component::SetIsActivating(bool state)
    {
        if (isEnabling)
        {
            if (state)
                Enable();
            else
                Disable();
        }
    }
}
