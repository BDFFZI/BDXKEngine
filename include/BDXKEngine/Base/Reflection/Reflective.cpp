#include "Reflective.h"

namespace BDXKEngine
{
    Type Reflective::GetTypeID(Transferer& transferer)
    {
        Type type;
        TransferFieldInfo(type);
        return type;
    }

    Type Reflective::GetTypeID() const
    {
        return BDXKEngine::GetTypeID(*this);
    }

    void Reflective::Transfer(Transferer& transferer)
    {
        Type type = GetTypeID();
        TransferFieldInfo(type);
    }
}
