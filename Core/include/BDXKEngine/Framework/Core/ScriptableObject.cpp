#include "ScriptableObject.h"
#include <iostream>
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEngine
{
    std::unordered_set<ScriptableObject*> ScriptableObject::allScriptableObjects;

    bool ScriptableObject::GetIsActivating() const
    {
        return true;
    }
    bool ScriptableObject::GetIsEnabling() const
    {
        return isEnabling;
    }
    void ScriptableObject::SetIsActivating(bool state)
    {
        if (isActivating == state)return;
        isActivating = state;

        if (GetInstanceID() == 92)
            printf("");

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

    bool ScriptableObject::IsNotResource() const
    {
        return !Serialization::IsPersistent(this);
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
        if (GetInstanceID() == 92)
            printf("");

        if (isAwakened == false)
        {
            if (const auto handler = dynamic_cast<AwakeHandler*>(this); handler != nullptr)
                if (IsNotResource()) handler->OnAwake();
            isAwakened = true;
        }

        if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
            if (IsNotResource()) handler->OnEnable();

        assert(allScriptableObjects.contains(this) == false); //事件函数执行异常
        allScriptableObjects.insert(this);

        //std::cout << "SwitchableObject::Enable " + std::to_string(GetInstanceID()) + " " + GetName() << std::endl;
    }
    void ScriptableObject::Disable()
    {
        if (GetInstanceID() == 92)
            printf("");

        assert(allScriptableObjects.contains(this)); //事件函数执行异常
        allScriptableObjects.erase(this);

        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            if (IsNotResource()) handler->OnDisable();

        //std::cout << "SwitchableObject::Disable " + std::to_string(GetInstanceID()) + " " + GetName() << std::endl;
    }
    void ScriptableObject::Awake()
    {
        Object::Awake();

        isActivating = GetIsActivating();
    }
    void ScriptableObject::PostAwake()
    {
        Object::PostAwake();

        if (IsActivatingAndEnabling() && isActivating)Enable();
    }
    void ScriptableObject::PreDestroy()
    {
        if (IsActivatingAndEnabling() && isActivating)Disable();

        if (isAwakened == true)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                if (IsNotResource()) handler->OnDestroy();
        }

        Object::PreDestroy();
    }
}
