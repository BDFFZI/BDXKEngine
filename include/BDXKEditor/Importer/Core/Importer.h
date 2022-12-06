#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Object/Guid/Guid.h"

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
        const Guid& GetGuid();
    private:
        inline static std::unordered_map<std::string, Type> assetsImporter = {};

        Guid guid;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
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
