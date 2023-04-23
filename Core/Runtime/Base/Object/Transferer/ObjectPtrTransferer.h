#pragma once
#include "Core/Runtime/Base/Reflection/Transferer.h"
#include "ObjectTransferer.h"

namespace BDXKEngine
{
    class Internal_ObjectPtrTransferer : public Transferer
    {
    public:
        const std::vector<ID>& GetObjectIDs() const;
        const std::vector<ObjectPtrBase*>& GetObjectPtrs() const;

        void TransferObjectPtr(ObjectPtrBase& objectPtrBase);

        Internal_ObjectPtrTransferer();

    protected:
        std::function<void(void*, const Type&)> GetTransferFunc(const Type& type) override;

    private:
        std::vector<ID> objectIDs;
        std::vector<ObjectPtrBase*> objectPtrs;
    };

    //使用该传送器传送一次ObjectPtr后，将会自动统计其引用的所有其他ObjectPtr（为空的ObjectPtr会自动跳过），并将该ObjectPtr自身插入到首位
    class ObjectPtrTransferer : public ObjectTransferer<Internal_ObjectPtrTransferer>
    {
    };
}
