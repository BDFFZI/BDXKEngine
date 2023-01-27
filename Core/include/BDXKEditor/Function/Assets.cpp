#include "Assets.h"
#include <filesystem>
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Function/Resources/Resources.h"

namespace BDXKEditor
{
    std::string Assets::GetRootPath()
    {
        return rootDirectory.substr(0, rootDirectory.size() - 1);
    }
    std::string Assets::GetAssetPath(const ObjectPtrBase& objectPtr)
    {
        const Guid guid = ObjectGuid::GetGuid(objectPtr.GetInstanceID());
        return guidToPath.contains(guid) ? guidToPath.at(guid) : "";
    }
    bool Assets::IsCanImport(const std::string& path)
    {
        return Importer::HasAssetsImporter(ParseFileExtension(path));
    }

    ObjectPtrBase Assets::Load(const std::string& path, bool reimport)
    {
        if (pathToGuid.contains(path) == false)
            LoadPathGuid();

        if (reimport == false && pathToGuid.contains(path))
        {
            const Guid guid = pathToGuid[path];
            const ID instance = ObjectGuid::GetInstanceID(guid);
            if (instance != 0)return Object::FindObjectOfInstanceID(instance);
            if (Resources::IsExisting(guid))return Resources::Load(guid);
        }

        //加载物体
        const ObjectPtr<Importer> importer = Importer::GetAssetsImporter(rootDirectory + path);
        const ObjectPtrBase object = importer->Import(rootDirectory + path);
        const Guid guid = importer->GetTargetGuid();

        //重新导入时可能需要修改Path和Guid的关联信息
        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        Resources::Save(guid, object); //用于非编辑器模式下的资源加载
        return object;
    }
    void Assets::Save(const std::string& path, ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull())
            throw std::exception("目标物体不存在");

        const ObjectPtr<Importer> importer = Importer::GetAssetsImporter(rootDirectory + path);
        importer->Export(rootDirectory + path, objectPtr);
        Resources::Save(importer->GetTargetGuid(), objectPtr); //用于非编辑器模式下的资源加载
    }
    bool Assets::IsExisting(const std::string& path)
    {
        return Serialization::IsExisting("Assets/" + path);
    }
    void Assets::LoadPathGuid()
    {
        pathToGuid.clear();
        guidToPath.clear();

        std::stack<std::filesystem::directory_entry> directories = {};
        directories.push(std::filesystem::directory_entry{GetRootPath()});
        while (directories.empty() == false)
        {
            auto iterator = std::filesystem::directory_iterator{directories.top()};
            directories.pop();
            for (const auto& item : iterator)
            {
                if (item.is_directory())
                    directories.push(item);
                else if (item.path().extension() == ".importer") //以importer提供的Guid和名称为准
                {
                    std::string path = item.path().string();

                    rapidjson::Document document = {};
                    document.Parse(ReadFile(path).c_str());
                    Guid guid = document["serialization_0"]["data"]["guid"].GetString();

                    path = path.substr(rootDirectory.size(), path.size() - 9 - rootDirectory.size());
                    pathToGuid[path] = guid;
                    guidToPath[guid] = path;
                }
            }
        }
    }

    void Assets::StaticConstructor()
    {
        if (std::filesystem::exists("Assets") == false)
            std::filesystem::create_directory("Assets");

        ObjectSerializerBase::AddDeserializeFallback([](const Guid& guid)
        {
            if (guidToPath.contains(guid))
            {
                const std::string path = rootDirectory + guidToPath[guid];
                if (std::filesystem::exists(path))
                    return Load(path.substr(rootDirectory.size()));
            }

            return ObjectPtrBase{};
        });
    }
}
