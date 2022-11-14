#pragma once
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "Core/Object.h"

namespace BDXKEngine
{
    template <typename TTransferer>
    class ObjectTransferer : public TTransferer
    {
    protected:
        void TransferObjectPtrBase(ObjectPtrBase& value)
        {
            throw std::exception("ObjectPtrBase的传输方式未定义");
        }
        CustomTransferFunc(ObjectPtrBase, TransferObjectPtrBase)

        void TransferValueFallback(void* value, const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos)
            {
                const auto objectPtrBase = static_cast<ObjectPtrBase*>(value);
                this->TransferValue(objectPtrBase, GetTypeID<ObjectPtrBase>());
                return;
            }

            TTransferer::TransferValueFallback(value, type);
        }
    };
}
