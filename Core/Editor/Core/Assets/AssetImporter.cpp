#include "AssetImporter.h"
#include <filesystem>
#include "Core/Runtime/Base/Object/Serialization/ObjectRef.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEditor
{
    ObjectPtr<AssetImporter> AssetImporter::GetAtPath(const std::filesystem::path& assetPath)
    {
        ObjectPtr<AssetImporter> importer;

        std::filesystem::path importerPath = assetPath;
        importerPath.concat(".importer");
        if (exists(importerPath))
        {
            //存在就直接加载
            auto jsonSerializer = ObjectSerialization::CreateJsonSerializer();
            importer = ObjectSerialization::Load<AssetImporter>(importerPath, jsonSerializer, xg::Guid{}, true);
            importer->path = assetPath;
        }
        else
        {
            //否则需要先创建
            const std::string extension = assetPath.extension().string();
            if (HasAssetsImporter(extension) == false)
            {
                Debug::LogWarning("不存在适用于目标类型的导入器");
                return {};
            }
            importer = Create(assetImporter[extension]).ToObject<AssetImporter>();
            importer->guid = xg::newGuid();
            importer->path = assetPath;
            importer->SaveImporter();
        }

        return importer;
    }
    void AssetImporter::SetAssetsImporter(const std::filesystem::path& extension, const Type& type)
    {
        assetImporter[extension] = type;
    }
    bool AssetImporter::HasAssetsImporter(const std::filesystem::path& extension)
    {
        return assetImporter.contains(extension);
    }

    const xg::Guid& AssetImporter::GetTargetGuid() const
    {
        return guid;
    }
    const std::filesystem::path& AssetImporter::GetTargetPath() const
    {
        return path;
    }

    void AssetImporter::SaveImporter() const
    {
        auto serializer = ObjectSerialization::CreateJsonSerializer();
        ObjectSerialization::Save(std::filesystem::path{path}.concat(".importer"), this, serializer, xg::Guid{});
    }
    ObjectPtrBase AssetImporter::ImportAsset()
    {
        //导入物体
        importObjects.clear();
        ObjectPtrBase rootObject = Import();

        //替换功能会导致ObjectID发生变化，从而导致指针失效，现在记录下所有相关指针，之后纠正
        ObjectPtrTransferer transferer;
        transferer.TransferObjectPtr(rootObject);
        rootObject->Transfer(transferer);

        //删除旧物体
        for (auto& [objectRef,objectPtr] : importObjects)
        {
            const ID objectID = ObjectRef::GetObjectID(objectRef);
            DestroyObject(FindObjectOfObjectID(objectID));
        }

        //换上新物体
        std::unordered_map<ID, ID> replaceState = {};
        for (auto& [objectRef,objectPtr] : importObjects)
        {
            const ID oldObjectID = ObjectRef::GetObjectID(objectRef);
            if (oldObjectID != 0)
            {
                replaceState[objectPtr.GetObjectID()] = oldObjectID;
                ReplaceObject(objectPtr, oldObjectID);
            }
            else
                ObjectRef::Add(objectRef, objectPtr.GetObjectID());
        }

        //纠正替换前的指针
        for (auto* objectPtr : transferer.GetObjectPtrs())
        {
            const ID newID = replaceState[objectPtr->GetObjectID()];
            if (newID != 0)
                *objectPtr = FindObjectOfObjectID(newID);
        }

        return rootObject;
    }

    void AssetImporter::BindObjectRef(ObjectPtrBase& objectPtr, const FileID& fileID)
    {
        const ObjectRef objectRef = {guid, fileID};
        importObjects.emplace_back(objectRef, objectPtr);
    }
    // void AssetImporter::BindObjectRef(std::vector<ObjectPtrBase>& objectPtrs, const std::vector<FileID>& fileIDs) const
    // {
    //     std::unordered_map<FileID, ID> oldObjectIDs = {};
    //
    //     //清除所有旧物体
    //     for (const FileID fileID : fileIDs)
    //     {
    //         const ObjectRef objectRef = {guid, fileID};
    //         if (const ID objectID = ObjectRef::GetObjectID(objectRef); FindObjectOfObjectID(objectID) != nullptr)
    //         {
    //             oldObjectIDs[fileID] = objectID;
    //             DestroyImmediate(FindObjectOfObjectID(objectID));
    //             ObjectRef::Remove(objectRef);
    //         }
    //     }
    //
    //     //替换为新物体
    //     const size_t count = objectPtrs.size();
    //     for (size_t i = 0; i < count; i++)
    //     {
    //         ReplaceObject(objectPtrs[i], oldObjectIDs[fileIDs[i]]);
    //         ObjectRef::Add({guid, fileIDs[i]}, objectPtrs[i].GetObjectID());
    //     }
    // }

    void AssetImporter::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(guid);
    }
}
