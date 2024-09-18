#pragma once
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Editor/Core/Assets/AssetImporter.h"
#include "Core/Runtime/CoreRuntime.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class AssetDatabase
    {
    public:
        static void AddRefreshDir(const std::string& name, const std::filesystem::path& path);
        static const std::vector<std::tuple<std::string, std::filesystem::path>>& GetAllRefreshDirs();
        static void Refresh(); //扫描素材文件夹，导入所有可导入但未导入的物体

        static std::filesystem::path GetAssetPath(const ObjectPtrBase& objectPtr);

        static bool IsCanImport(const std::filesystem::path& path);

        //优先返回内存中的，再者从SceneResources文件夹加载，最后没办法再从素材库中加载
        static ObjectPtrBase Load(const std::filesystem::path& path);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::filesystem::path& path)
        {
            return Load(path).ToObject<TObject>();
        }
        //直接从素材库中重新导入并加载
        static ObjectPtrBase ReLoad(const std::filesystem::path& path);
        template <typename TObject>
        static ObjectPtr<TObject> ReLoad(const std::filesystem::path& path)
        {
            return ReLoad(path).ToObject<TObject>();
        }


        static void Save(const std::filesystem::path& path, const ObjectPtrBase& objectPtr);

    private:
        inline static std::vector<std::tuple<std::string, std::filesystem::path>> refreshDirs = {};
        inline static std::unordered_map<std::filesystem::path, xg::Guid> pathToGuid = {};
        inline static std::unordered_map<xg::Guid, std::filesystem::path> guidToPath = {};

        static ObjectPtrBase Import(const std::filesystem::path& path);

        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, -10)
    };
}
