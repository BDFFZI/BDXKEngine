#include "SwitchableObject.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEngine
{
    bool SwitchableObject::GetIsEnabling() const
    {
        return isEnabling;
    }
    void SwitchableObject::SetIsEnabling(const bool state)
    {
        if (isEnabling == state)return;
        isEnabling = state;

        if (GetIsActivating() == false)return;
        if (state)Enable();
        else Disable();
    }
    bool SwitchableObject::IsNotResource() const
    {
        return !ObjectSerialization::IsPersistent(this);
    }
    bool SwitchableObject::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }

    bool SwitchableObject::GetIsActivating() const
    {
        return true;
    }
    void SwitchableObject::UpdateActivating()
    {
        if (GetIsActivating() == lastIsActivating)
            return; //活性状态实际并没有改变，跳过（因为Enable和Disable并不是总在状态改变时才触发（创建销毁时也会触发一次），所以状态未改变的情况下也可能调用，需要检查）
        lastIsActivating = !lastIsActivating;

        if (isEnabling == false)return;
        if (lastIsActivating)Enable();
        else Disable();
    }

    void SwitchableObject::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(isEnabling);
    }

    void SwitchableObject::Enable()
    {
        //std::cout << "SwitchableObject::Enable " + std::to_string(GetObjectID()) + " " + GetName() << std::endl;
    }
    void SwitchableObject::Disable()
    {
        //std::cout << "SwitchableObject::Disable " + std::to_string(GetObjectID()) + " " + GetName() << std::endl;
    }

    void SwitchableObject::Awake()
    {
        Object::Awake();

        lastIsActivating = GetIsActivating();
    }
    void SwitchableObject::Start()
    {
        Object::Start();

        if (IsActivatingAndEnabling())
            Enable();
    }
    void SwitchableObject::Destroy()
    {
        if (IsActivatingAndEnabling())
            Disable();

        Object::Destroy();
    }
}
