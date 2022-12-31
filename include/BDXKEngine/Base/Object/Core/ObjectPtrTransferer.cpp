#include "ObjectPtrTransferer.h"

namespace BDXKEngine
{
    std::vector<int> Internal_ObjectPtrTransferer::GetReferences() const
    {
        std::vector result = {references};
        std::erase(result, headInstanceID);
        result.insert(result.begin(), headInstanceID);
        return result;
    }
    void Internal_ObjectPtrTransferer::SetHeadInstanceID(int root)
    {
        headInstanceID = root;
    }
    void Internal_ObjectPtrTransferer::TransferObjectPtr(const ObjectPtrBase& objectPtrBase)
    {
        Object* object = objectPtrBase.ToObjectBase();
        if (object != nullptr)
        {
            if (std::ranges::find(references, object->GetInstanceID()) == references.end())
            {
                references.insert(references.end(), object->GetInstanceID());
                object->Transfer(*this);
            }
            else
            {
                std::erase(references, object->GetInstanceID());
                references.insert(references.end(), object->GetInstanceID());
            }
        }
    }
    void Internal_ObjectPtrTransferer::TransferValue(void* value, const Type& type)
    {
        if (type.find("ObjectPtr") == std::string::npos)
            return;

        Transferer::TransferValue(value, type);
    }
    ObjectPtrTransferer::ObjectPtrTransferer(const ObjectPtrBase& root)
    {
        SetHeadInstanceID(root.GetInstanceID());
    }
}
