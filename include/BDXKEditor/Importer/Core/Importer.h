#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    std::string ParseFileName(const std::string& path);

    class Importer : public Object
    {
    public:
        static ObjectPtr<Importer> GetAssetsImporter(const std::string& extension);
        static void SetAssetsImporter(const std::string& extension, const Type& type);

        virtual ObjectPtrBase Import(std::string filePath) = 0;
    private:
        inline static std::unordered_map<std::string, Type> assetsImporter = {};
    };

    struct CustomAssetsImporterRegister
    {
        CustomAssetsImporterRegister(const std::string& extension, const Type& type)
        {
            Importer::SetAssetsImporter(extension, type);
        }
    };

#define CustomAssetsImporter(extension,type) inline CustomAssetsImporterRegister CustomAssetsImporter##extension##type(#extension,GetTypeOf<type>());
}
