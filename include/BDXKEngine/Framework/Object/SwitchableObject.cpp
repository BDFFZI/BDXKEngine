#include "SwitchableObject.h"
#include "SwitchableObjectEvent.h"

namespace BDXKEngine
{
    bool SwitchableObject::GetIsActivating() const
    {
        return true;
    }
    bool SwitchableObject::GetIsEnabling() const
    {
        return isEnabling;
    }

    void SwitchableObject::SetIsActivating()
    {
        if (GetIsEnabling())
        {
            if (GetIsActivating())
                Enable();
            else
                Disable();
        }
    }
    void SwitchableObject::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;

        isEnabling = state;

        if (GetIsActivating())
        {
            if (isEnabling)
                Enable();
            else
                Disable();
        }
    }

    bool SwitchableObject::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }

    std::string SwitchableObject::ToString()
    {
        std::stringstream stream;
        stream << Object::ToString();
        stream << "激活中：" << GetIsActivating() << std::endl;
        stream << "启用中：" << GetIsEnabling() << std::endl;
        return stream.str();
    }


    void SwitchableObject::Enable()
    {
        if (isAwakened == false)
        {
            if (const auto handler = dynamic_cast<AwakeHandler*>(this); handler != nullptr)
                handler->OnAwake();
            isAwakened = true;
        }

        if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
            handler->OnEnable();

        Debug::LogWarning("SwitchableObject::Enable " + std::to_string(GetInstanceID()));
    }
    void SwitchableObject::Disable()
    {
        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            handler->OnDisable();

        Debug::LogWarning("SwitchableObject::Disable " + std::to_string(GetInstanceID()));
    }
    void SwitchableObject::Awake()
    {
        Object::Awake();

        if (IsActivatingAndEnabling())Enable();
    }
    void SwitchableObject::Destroy()
    {
        if (isAwakened)
        {
            if (IsActivatingAndEnabling())Disable();

            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                handler->OnDestroy();
        }

        Object::Destroy();
    }
    void SwitchableObject::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        TransferProperty(IsEnabling)
    }
}
