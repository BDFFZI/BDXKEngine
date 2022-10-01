#include "SwitchableObject.h"

namespace BDXKEngine
{
    bool SwitchableObject::GetIsEnabling() const
    {
        return isEnabling;
    }

    bool SwitchableObject::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }

    void SwitchableObject::UpdateActivating()
    {
        OnUpdateActivating(GetIsActivating());
    }
    void SwitchableObject::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        bool isEnabling = this->isEnabling;
        transferrer.TransferBool(nameof(isEnabling), isEnabling);
        SetIsEnabling(isEnabling);
    }

    std::string SwitchableObject::ToString()
    {
        std::stringstream stream;
        stream << Object::ToString();
        stream << "激活中：" << GetIsActivating() << std::endl;
        stream << "启用中：" << GetIsEnabling() << std::endl;
        return stream.str();
    }
}
