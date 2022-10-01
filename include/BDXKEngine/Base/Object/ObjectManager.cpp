#include "ObjectManager.h"
#include "Object.h"

namespace BDXKEngine
{
    void ObjectManager::FlushDestroyQueue()
    {
        Object::FlushDestroyQueue();
    }

    std::string ObjectManager::InstanceIDToGuid(unsigned int instanceID)
    {
        return {};
    }

    unsigned int ObjectManager::GuidToInstanceID(const std::string& guid)
    {
        return 0;
    }
}
