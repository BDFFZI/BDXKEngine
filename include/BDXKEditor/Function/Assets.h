#pragma once
#include <string>
#include "BDXKEditor/Importer/Core/Importer.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtrBase.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Assets
    {
    public:
        static std::string GetRootPath();
        
        static ObjectPtrBase Load(const std::string& path,bool reimport = false);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path)
        {
            return Load(path).ToObject<TObject>();
        }
    private:
        inline static std::string rootDirectory = "Assets/";
        inline static std::unordered_map<std::string, Guid> pathToGuid = {};
        inline static std::unordered_map<Guid, std::string> guidToPath = {};
        
        static void LoadPathGuid();
        static ObjectPtr<Importer> LoadImporter(const std::string& path);
        
        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
