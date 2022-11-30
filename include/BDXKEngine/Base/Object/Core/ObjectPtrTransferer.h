#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "ObjectPtrBase.h"

namespace BDXKEngine
{
    //TODO 无法传送容器内的指针
    class ObjectPtrTransferer : public Transferer
    {
    public:
        ObjectPtrTransferer(int root);
        
        std::vector<int> GetReferences();
    protected:
        void TransferValue(void* value, const Type& type) override;
    private:
        std::unordered_set<int> references;
    };
}
