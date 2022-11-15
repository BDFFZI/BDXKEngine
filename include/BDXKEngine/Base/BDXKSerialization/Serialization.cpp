#include "Serialization.h"

namespace BDXKEngine
{
    Type Serialization::GetTypeID()
    {
        return typeid(*this).name();
    }
    void Serialization::Transfer(Transferer& transferer)
    {
    }
}
