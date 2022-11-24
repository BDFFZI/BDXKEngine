#pragma once
#include <regex>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "Core/Object.h"

namespace BDXKEngine
{
    template <typename TIOTransferer>
    class ObjectTransferer : public TIOTransferer
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
                    if (this->IsImporter())*static_cast<std::uintptr_t*>(value) = *reinterpret_cast<std::uintptr_t*>(&null);
                    const auto objectPtrBase = static_cast<ObjectPtrBase*>(value);
                    this->TransferValue(objectPtrBase, GetTypeIDOf<ObjectPtrBase>());
                };
            }

            return TIOTransferer::GetTransferFuncFallback(type);
        }
        int GetTypeSizeFallback(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
            {
                return sizeof(ObjectPtr<Object>);
            }

            return TIOTransferer::GetTypeSizeFallback(type);
        }
    private:
        ObjectPtr<Object> null;
    };
}
