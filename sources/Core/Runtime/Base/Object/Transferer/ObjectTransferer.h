#pragma once
#include <regex>
#include "Core/Runtime/Base/Object/Pointer/ObjectPtr.h"
#include "Core/Runtime/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    //使用该传送器传输数据时能自动识别出所有ObjectPtr类型，并将其以ObjectPtrBase类型回调
    template <typename TTransferer>
    class ObjectTransferer : public TTransferer
    {
    protected:
        std::function<void(void*, const Type&)> GetTransferFunc(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
                return TTransferer::GetTransferFunc(objectPtrType);

            return TTransferer::GetTransferFunc(type);
        }
        int GetTransferSize(const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos && type.find("std::vector") == std::string::npos)
                return TTransferer::GetTransferSize(objectPtrType);

            return TTransferer::GetTransferSize(type);
        }

    private:
        inline static Type objectPtrType = GetTypeOf<ObjectPtrBase>();
    };
}
