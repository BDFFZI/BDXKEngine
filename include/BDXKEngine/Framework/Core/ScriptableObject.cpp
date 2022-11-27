#include "ScriptableObject.h"
#include <iostream>
#include "BDXKEngine/Platform/Resources/Resources.h"

namespace BDXKEngine
{
    std::unordered_set<ScriptableObject*> ScriptableObject::allScriptableObjects;

    bool ScriptableObject::GetIsActivating() const
    {
        return Resources::IsResource(this) == false;
    }
    bool ScriptableObject::GetIsEnabling() const
    {
        return isEnabling;
    }

    void ScriptableObject::SetIsActivating(bool state)
    {
        if (isActivating == state)return;
        isActivating = state;

        if (isEnabling == false)return;
        if (state)Enable();
        else Disable();
    }
    void ScriptableObject::SetIsEnabling(bool state)
    {
        if (isEnabling == state)return;
        isEnabling = state;

        if (GetIsActivating() == false)return;
        if (state)Enable();
        else Disable();
    }

    bool ScriptableObject::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }

    void ScriptableObject::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(isEnabling);
    }

    void ScriptableObject::Enable()
    {
        if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
            handler->OnEnable();

        allScriptableObjects.insert(this);

        std::cout << "SwitchableObject::Enable " + std::to_string(GetInstanceID()) + " " + GetName() << std::endl;
    }
    void ScriptableObject::Disable()
    {
        allScriptableObjects.erase(this);

        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            handler->OnDisable();

        std::cout << "SwitchableObject::Disable " + std::to_string(GetInstanceID()) + " " + GetName() << std::endl;
    }
    void ScriptableObject::Awake()
    {
        Object::Awake();

        isActivating = GetIsActivating();

        if (isActivating == true)
        {
            if (const auto handler = dynamic_cast<AwakeHandler*>(this); handler != nullptr)
                handler->OnAwake();
            isAwakened = true;
        }

        if (IsActivatingAndEnabling())Enable();
    }
    void ScriptableObject::Destroy()
    {
        if (isAwakened == true)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                handler->OnDestroy();
        }

        if (IsActivatingAndEnabling())Disable();

        Object::Destroy();
    }
}
