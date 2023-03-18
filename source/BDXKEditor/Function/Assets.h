#pragma once
#include <string>
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEditor/Base/Importer/Core/Importer.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Assets
    {
    public:
        static std::string GetRootPath();
        static std::string GetAssetPath(const ObjectPtrBase& objectPtr);

        static bool IsCanImport(const std::string& path);
        static bool IsExisting(const std::string& path);

        static ObjectPtrBase Load(const std::string& path, bool reimport = false);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path, bool reimport = false)
        {
            return Load(path, reimport).ToObject<TObject>();
        }
        static void Save(const std::string& path, ObjectPtrBase& objectPtr);
    private:
        inline static std::string rootDirectory = "Assets/";
        //下方路径信息都是相对Assets的路径
        inline static std::unordered_map<std::string, Guid> pathToGuid = {};
        inline static std::unordered_map<Guid, std::string> guidToPath = {};

        static void LoadPathGuid();


        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
