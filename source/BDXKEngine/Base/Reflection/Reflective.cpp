#include "Reflective.h"

namespace BDXKEngine
{
    Type Reflective::GetType(Transferer& transferer)
    {
        Type type;
        TransferFieldInfo(type);
        return type;
    }

    Type Reflective::GetType() const
    {
        return BDXKEngine::GetType(*this);
    }

    void Reflective::Transfer(Transferer& transferer)
    {
        Type type = GetType();
        TransferFieldInfo(type);
    }
}
