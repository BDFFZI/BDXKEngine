#pragma once
#include <filesystem>
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectRef.h"
#include "Core/Runtime/Base/Serializer/Json/JsonReader.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    /**
     * \brief 素材导入器
     * 导入器不是将物体直接导入到游戏，而是负责将素材转换换成引擎可识别的数据。
     * 接着Assets会将导入器返回的数据直接用SceneResources保存，直到实际使用时才会加载。
     * 加载和导入是不一样的。加载会触发实例化，因为确实需要使用。而导入只是资源的转换，不需要实例化。
     */
    class AssetImporter : public Object
    {
    public:
        static ObjectPtr<AssetImporter> GetAtPath(const std::filesystem::path& assetPath);
        static void SetAssetsImporter(const std::filesystem::path& extension, const Type& type);
        static bool HasAssetsImporter(const std::filesystem::path& extension);

        const xg::Guid& GetTargetGuid() const;
        const std::filesystem::path& GetTargetPath() const;

        void SaveImporter() const;
        virtual ObjectPtrBase ImportAsset();

    protected:
        void BindObjectRef(ObjectPtrBase& objectPtr, const FileID& fileID);
        virtual ObjectPtrBase Import() = 0;

    private:
        inline static std::unordered_map<std::filesystem::path, Type> assetImporter = {};

        std::filesystem::path path;
        std::vector<std::tuple<ObjectRef, ObjectPtrBase>> importObjects;

        xg::Guid guid;
        void Transfer(Transferer& transferer) override;
    };

    struct CustomAssetsImporterRegister
    {
        CustomAssetsImporterRegister(const std::string& extension, const Type& type)
        {
            AssetImporter::SetAssetsImporter(extension, type);
        }
    };

#define CustomAssetsImporter(extension,type) inline CustomAssetsImporterRegister CustomAssetsImporter##extension##type("."#extension,GetTypeOf<type>());
}
