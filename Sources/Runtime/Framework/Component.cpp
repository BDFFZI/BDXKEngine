#include "Component.h"
#include "GameObject.h"


namespace BDXKEngine
{
    ObjectPtr<GameObject> Component::GetGameObject() { return gameObject; }
    ObjectPtr<Transform> Component::GetTransform() { return GetGameObject()->GetComponent<Transform>(); }

    bool Component::GetIsEnabling() const
    {
        return isEnabling;
    }

    void Component::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;
        
        isEnabling = state;
        
        if (gameObject->GetIsActivatingInHierarchy() == false)
            return;
        
        if (GetIsInstantiating())
        {
            if (state)
                Enable();
            else
                Disable();
        }
    }


    void Component::Export(Exporter& exporter)
    {
        Object::Export(exporter);

        exporter.TransferObject(gameObject);
    }

    void Component::Import(Importer& importer)
    {
        Object::Import(importer);

        gameObject = importer.TransferObject();
    }

    void Component::Awake()
    {
        Object::Awake();

        gameObject->components.emplace_back(this);
    }

    void Component::Destroy()
    {
        gameObject->components.erase(std::find(gameObject->components.begin(), gameObject->components.end(),
                                               ObjectPtr<Component>{this}));

        Object::Destroy();
    }

    void Component::Enable()
    {
    }

    void Component::Disable()
    {
    }
}
