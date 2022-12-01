#include "ObjectPtrTransferer.h"

namespace BDXKEngine
{
    std::vector<int> Internal_ObjectPtrTransferer::GetReferences()
    {
        return {references.begin(), references.end()};
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
        TransferObjectPtr(root);
    }
}
