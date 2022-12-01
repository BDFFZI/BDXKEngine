#pragma once
#include <regex>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "../Object.h"

namespace BDXKEngine
{
    template <typename TTransferer>
    class ObjectTransferer : public TTransferer
    {
    protected:
        std::function<void(void*, const Type&)> GetTransferFuncFallback(const Type& type) override
        {
            if (this->IsContainTransferFunc(GetTypeOf<ObjectPtrBase>()) == false) throw std::exception("ObjectPtrBase的传送方式未定义");
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
            {
                return [this](void* value, const Type& type)
                {
                    ObjectPtrBase::GetVirtualTable(type, static_cast<ObjectPtrBase*>(value));
                    this->TransferValue(value, GetTypeOf<ObjectPtrBase>());
                };
            }

            return TTransferer::GetTransferFuncFallback(type);
        }
        int GetTypeSizeFallback(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
            {
                return sizeof ObjectPtr<Object>;
            }

            return TTransferer::GetTypeSizeFallback(type);
        }
    };
}
