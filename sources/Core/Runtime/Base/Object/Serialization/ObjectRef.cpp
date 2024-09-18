#include "ObjectRef.h"
#include <format>
#include <climits>
#include <random>

namespace BDXKEngine
{
    ObjectRef ObjectRef::Null()
    {
        return {};
    }
    FileID ObjectRef::GenerateFileID(const xg::Guid guid)
    {
        const auto seed = static_cast<unsigned int>(time(nullptr));
        std::default_random_engine engine{seed};
        std::uniform_int_distribution<> random(1, INT_MAX);

        FileID fileID;
        do fileID = random(engine);
        while (guidToFileIDs[guid].contains(fileID));

        return fileID;
    }
    std::unordered_map<ObjectRef, ID>& ObjectRef::GetAll()
    {
        return objectRefToObjectID;
    }

    ObjectRef ObjectRef::GetObjectRef(const ID objectID)
    {
        const auto objectRef = objectIDToObjectRef.find(objectID);
        if (objectRef != objectIDToObjectRef.end())
            return objectRef->second;

        return ObjectRef{};
    }
    ID ObjectRef::GetObjectID(const ObjectRef objectRef)
    {
        const auto objectID = objectRefToObjectID.find(objectRef);
        if (objectID != objectRefToObjectID.end())
            return objectID->second;

        return 0;
    }

    ID ObjectRef::GetRootObjectID(const xg::Guid& guid)
    {
        return guidToRootObjectID[guid];
    }
    void ObjectRef::MarkRoot(const ObjectRef& objectRef)
    {
        guidToRootObjectID[objectRef.guid] = GetObjectID(objectRef);
    }
    void ObjectRef::MarkRoot(const ID& id)
    {
        guidToRootObjectID[GetObjectRef(id).guid] = id;
    }
    bool ObjectRef::IsRoot(const ObjectRef& objectRef)
    {
        return guidToRootObjectID[objectRef.guid] == GetObjectID(objectRef);
    }

    void ObjectRef::Add(const ObjectRef objectRef, const ID objectID)
    {
        if (objectRefToObjectID.contains(objectRef)) //重复绑定
            throw std::exception(std::format("当前ObjectRef已被绑定 %i", objectRefToObjectID.at(objectRef)).c_str());

        //允许更换物体的ObjectRef，以便另存为操作，另外编辑器下创建物体会自动绑定一个ObjectRef
        if (objectIDToObjectRef.contains(objectID))
            objectRefToObjectID.erase(objectIDToObjectRef.at(objectID));

        objectRefToObjectID[objectRef] = objectID;
        objectIDToObjectRef[objectID] = objectRef;
        guidToFileIDs[objectRef.guid].insert(objectRef.fileID);
    }
    void ObjectRef::Remove(const ID objectID)
    {
        const ObjectRef objectRef = objectIDToObjectRef[objectID];
        objectRefToObjectID.erase(objectRef);
        objectIDToObjectRef.erase(objectID);
        guidToFileIDs[objectRef.guid].erase(objectRef.fileID);
    }
    void ObjectRef::Remove(const ObjectRef objectRef)
    {
        objectIDToObjectRef.erase(objectRefToObjectID[objectRef]);
        objectRefToObjectID.erase(objectRef);
        guidToFileIDs[objectRef.guid].erase(objectRef.fileID);
    }
    void ObjectRef::Remove(const xg::Guid guid)
    {
        for (const auto fileID : guidToFileIDs[guid])
        {
            ObjectRef objectRef = {guid, fileID};
            objectIDToObjectRef.erase(objectRefToObjectID[objectRef]);
            objectRefToObjectID.erase(objectRef);
        }
        guidToFileIDs.erase(guid);
    }
    bool ObjectRef::IsNull() const
    {
        return guid.isValid() == false && fileID == 0;
    }

    void ObjectRef::Transfer(Transferer& transferer)
    {
        TransferFieldInfo(guid);
        TransferFieldInfo(fileID);
    }
    std::string ObjectRef::ToString() const
    {
        return std::format("{{{},{}}}", guid.str(), fileID);
    }
}

size_t std::hash<BDXKEngine::ObjectRef>::operator()(BDXKEngine::ObjectRef const& objectRef) const noexcept
{
    return hash<xg::Guid>()(objectRef.guid);
}
