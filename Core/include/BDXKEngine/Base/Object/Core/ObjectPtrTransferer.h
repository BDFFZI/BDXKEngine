#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "ObjectTransferer.h"

namespace BDXKEngine
{
    class Internal_ObjectPtrTransferer : public Transferer
    {
    public:
        std::vector<int> GetReferences() const;
    protected:
        void SetHeadInstanceID(int root);
        void TransferValue(void* value, const Type& type) override;
        void TransferObjectPtr(const ObjectPtrBase& objectPtrBase);
        CustomTransferFunc(ObjectPtrBase, TransferObjectPtr)
    private:
        int headInstanceID = 0;
        std::vector<int> references;
    };

    class ObjectPtrTransferer : public ObjectTransferer<Internal_ObjectPtrTransferer>
    {
    public:
        ObjectPtrTransferer(const ObjectPtrBase& root);
    };
}
