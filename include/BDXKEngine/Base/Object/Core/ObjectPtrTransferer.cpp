#include "ObjectPtrTransferer.h"

namespace BDXKEngine
{
    std::vector<int> Internal_ObjectPtrTransferer::GetReferences()
    {
        references.erase(headInstanceID);
        std::vector<int> result = {};
        result.insert(result.end(), headInstanceID);
        result.insert(result.end(), references.begin(), references.end());
        return result;
    }
    void Internal_ObjectPtrTransferer::SetHeadInstanceID(int root)
    {
        headInstanceID = root;
    }
    void Internal_ObjectPtrTransferer::TransferObjectPtr(const ObjectPtrBase& objectPtrBase)
    {
        Object* object = objectPtrBase.ToObjectBase();
        if (object != nullptr && references.contains(object->GetInstanceID()) == false)
        {
            references.insert(object->GetInstanceID());
            object->Transfer(*this);
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
