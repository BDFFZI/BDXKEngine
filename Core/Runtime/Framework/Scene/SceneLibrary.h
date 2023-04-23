#pragma once
#include "Core/Runtime/Base/Object/Serialization/ObjectSerializer.h"

namespace BDXKEngine
{
    /**
     * 对ObjectSerialization的封装，增加了对Resources文件夹的支持。（不等于Unity中的Resources文件夹，而是等于Library/Artifacts文件夹）
     * 该文件夹的文件名均为其Guid，因为会注册反序列化回调，这样可以便于查找多个引用物体。
     * 引擎的启动起点是加载场景，确保场景里用到的所有外部物体都在该文件夹内
     */
    class SceneLibrary
    {
    public:
        static std::filesystem::path ParsePath(const std::string& fileName);
        
        static ObjectPtrBase Load(const xg::Guid& guid);
        static void Save(const xg::Guid& guid, const ObjectPtrBase& objectPtr);
        static bool IsExisting(const xg::Guid& guid);

    private:
        static ObjectPtrBase LoadOfFileSystem(const std::filesystem::path& path);
        static void SaveOfFileSystem(const std::filesystem::path& path, const ObjectPtrBase& objectPtr);

        static ObjectPtrBase FindSerializationFallback(const xg::Guid& guid);

        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
