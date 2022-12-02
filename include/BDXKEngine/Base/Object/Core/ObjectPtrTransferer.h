#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "ObjectTransferer.h"

namespace BDXKEngine
{
    //TODO 无法传送容器内的指针
    class Internal_ObjectPtrTransferer : public Transferer
    {
    public:
        std::vector<int> GetReferences();
    protected:
        void TransferValue(void* value, const Type& type) override;
        void TransferObjectPtr(const ObjectPtrBase& objectPtrBase);
        CustomTransferFunc(ObjectPtrBase, TransferObjectPtr)
    private:
        std::unordered_set<int> references;
    };

    class ObjectPtrTransferer : public ObjectTransferer<Internal_ObjectPtrTransferer>
    {
    public:
        ObjectPtrTransferer(const ObjectPtrBase& root);
    };
}
