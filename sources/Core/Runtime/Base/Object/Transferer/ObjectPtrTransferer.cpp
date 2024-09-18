#include "ObjectPtrTransferer.h"
#include "Core/Runtime/Base/Object/Object.h"

namespace BDXKEngine
{
    const std::vector<ID>& Internal_ObjectPtrTransferer::GetObjectIDs() const
    {
        return objectIDs;
    }
    const std::vector<ObjectPtrBase*>& Internal_ObjectPtrTransferer::GetObjectPtrs() const
    {
        return objectPtrs;
    }
    void Internal_ObjectPtrTransferer::TransferObjectPtr(ObjectPtrBase& objectPtrBase)
    {
        Object* object = objectPtrBase.ToObject();
        if (object != nullptr)
        {
            if (std::ranges::find(objectIDs, object->GetObjectID()) == objectIDs.end())
            {
                //发现新的物体，记录ID并递归遍历
                objectIDs.insert(objectIDs.end(), object->GetObjectID());
                object->Transfer(*this);
            }

            objectPtrs.emplace_back(&objectPtrBase);
        }
    }

    Internal_ObjectPtrTransferer::Internal_ObjectPtrTransferer()
    {
        SetTransfer<ObjectPtrBase>([this](ObjectPtrBase& objectPtrBase) { TransferObjectPtr(objectPtrBase); });
    }

    std::function<void(void*, const Type&)> Internal_ObjectPtrTransferer::GetTransferFunc(const Type& type)
    {
        if (type.find("ObjectPtr") == std::string::npos)
            return [](void*, const Type&)
            {
            };

        return Transferer::GetTransferFunc(type);
    }
}
