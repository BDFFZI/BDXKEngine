#include "ObjectSwitchable.h"

namespace BDXKEngine
{
    bool ObjectSwitchable::GetIsEnabling() const
    {
        return isEnabling;
    }

    bool ObjectSwitchable::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }
    
    std::wstring ObjectSwitchable::ToString()
    {
        std::wstringstream stream;
        stream << Object::ToString();
        stream << L"激活中：" << GetIsActivating() << std::endl;
        stream << L"启用中：" << GetIsEnabling() << std::endl;
        return stream.str();
    }
}
