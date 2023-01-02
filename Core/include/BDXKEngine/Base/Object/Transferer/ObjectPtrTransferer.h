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

    //使用该传送器传送一次ObjectPtr后，将会自动统计其引用的所有其他ObjectPtr，并将该ObjectPtr自身插入到首位
    class ObjectPtrTransferer : public ObjectTransferer<Internal_ObjectPtrTransferer>
    {
    public:
        ObjectPtrTransferer(const ObjectPtrBase& root);
    };
}
