#include "ScriptableObject.h"
#include <iostream>
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEngine
{
    std::unordered_set<ScriptableObject*> ScriptableObject::allScriptableObjects;


    bool ScriptableObject::GetIsEnabling() const
    {
        return isEnabling;
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

    bool ScriptableObject::GetIsActivating() const
    {
        return true;
    }
    void ScriptableObject::UpdateActivating()
    {
        if (GetIsActivating() == lastIsActivating)
            return; //活性状态实际并没有改变，跳过（因为Enable和Disable并不是总在状态改变时才触发（创建销毁时会额外触发），加上GameObject和Component的关系，导致会多次调用）
        lastIsActivating = !lastIsActivating;
        
        if (isEnabling == false)return;
        if (lastIsActivating)Enable();
        else Disable();
    }

    void ScriptableObject::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(isEnabling);
    }
    void ScriptableObject::Enable()
    {
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
        assert(allScriptableObjects.contains(this)); //事件函数执行异常
        allScriptableObjects.erase(this);

        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            if (IsNotResource()) handler->OnDisable();

        //std::cout << "SwitchableObject::Disable " + std::to_string(GetInstanceID()) + " " + GetName() << std::endl;
    }
    void ScriptableObject::Awake()
    {
        Object::Awake();

        lastIsActivating = GetIsActivating();
    }
    void ScriptableObject::PostAwake()
    {
        Object::PostAwake();

        if (IsActivatingAndEnabling())Enable();
    }
    void ScriptableObject::PreDestroy()
    {
        if (IsActivatingAndEnabling())Disable();

        if (isAwakened == true)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                if (IsNotResource()) handler->OnDestroy();
        }

        Object::PreDestroy();
    }
}
