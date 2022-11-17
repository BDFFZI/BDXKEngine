#include "ObjectPtrTransferer.h"
#include "Object.h"
#include "ObjectPtrBase.h"

namespace BDXKEngine
{
    ObjectPtrTransferer::ObjectPtrTransferer(int root): root(root)
    {
        references.insert(root);
    }
    void ObjectPtrTransferer::Reset(std::string& data)
    {
        references.clear();
        references.insert(root);
    }
    std::vector<int> ObjectPtrTransferer::GetReferences()
    {
        return {references.begin(), references.end()};
    }
    void ObjectPtrTransferer::TransferValue(void* value, const Type& type)
    {
        if (type.find("ObjectPtr") != std::string::npos)
        {
            Object* object = static_cast<ObjectPtrBase*>(value)->ToObjectBase();
            if (object != nullptr && references.count(object->GetInstanceID()) == 0)
            {
                references.insert(object->GetInstanceID());
                object->Transfer(*this);
            }
        }
    }
}
