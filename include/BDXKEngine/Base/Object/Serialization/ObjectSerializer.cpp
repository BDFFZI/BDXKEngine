#include "ObjectSerializer.h"

namespace BDXKEngine
{
    void ObjectSerializerAdapter::TransferSerialization(Transferer& transferer, std::string& serialization)
    {
        transferer.TransferField("data", serialization);
    }
    std::string ObjectSerializerAdapter::LoadSerialization(const Guid& guid)
    {
        throw std::exception("前置资源未加载");
    }
}
