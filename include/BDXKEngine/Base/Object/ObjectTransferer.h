#pragma once
#include <regex>

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

        std::function<void(void*, const Type&)> GetTransferFuncFallback(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
            {
                return [this](void* value, const Type& type)
                {
                    const auto objectPtrBase = static_cast<ObjectPtrBase*>(value);
                    this->TransferValue(objectPtrBase, GetTypeID<ObjectPtrBase>());
                };
            }

            return TTransferer::GetTransferFuncFallback(type);
        }
        int GetTypeSizeFallback(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
            {
                return sizeof(ObjectPtr<Object>);
            }

            return TTransferer::GetTypeSizeFallback(type);
        }
    };
}
