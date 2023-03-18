#pragma once
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Base/Data/Guid/Guid.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    std::string ParseFileName(const std::string& path);
    std::string ParseFileExtension(const std::string& path);

    /**
     * \brief 虽然叫导入器，但也负责导出工作
     */
    class Importer : public Object
    {
    public:
        static ObjectPtr<Importer> GetAssetsImporter(const std::string& path);
        static void SetAssetsImporter(const std::string& extension, const Type& type);
        static bool HasAssetsImporter(const std::string& extension);

        const Guid& GetTargetGuid();

        ObjectPtrBase Import(const std::string& filePath);
        void Export(const std::string& filePath, ObjectPtrBase& objectPtr);
    protected:
        virtual ObjectPtrBase ImportAsset(const std::string& filePath) = 0;
        virtual void ExportAsset(const std::string& filePath, const ObjectPtrBase& objectPtr);
    private:
        inline static std::unordered_map<std::string, Type> assetImporter = {};

        Guid guid;

        void RectifyGuid(ObjectPtrBase& objectPtr);
        void Transfer(Transferer& transferer) override;
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
