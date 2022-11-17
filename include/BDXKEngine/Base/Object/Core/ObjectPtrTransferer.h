#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "ObjectPtrBase.h"

namespace BDXKEngine
{
    class ObjectPtrTransferer : public Transferer
    {
    public:
        ObjectPtrTransferer(int root);

        void Reset(std::string& data) override;
        std::vector<int> GetReferences();
    protected:
        void TransferValue(void* value, const Type& type) override;
    private:
        std::unordered_set<int> references;
        int root = 0;
    };
}
