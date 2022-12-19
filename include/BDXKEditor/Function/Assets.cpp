#include "Assets.h"
#include <filesystem>

#include "BDXKEngine/Platform/Resources/Resources.h"

namespace BDXKEditor
{
    std::string Assets::GetRootPath()
    {
        return rootDirectory.substr(0, rootDirectory.size() - 1);
    }

    ObjectPtrBase Assets::Load(const std::string& path, bool reimport)
    {
        if (pathToGuid.contains(path) == false)
            LoadPathGuid();

        if (reimport == false && pathToGuid.contains(path))
        {
            const Guid guid = pathToGuid[path];
            const int instance = ObjectGuid::GetInstanceID(guid);
            if (instance != 0)return Object::FindObjectOfInstanceID(instance);
            if (Resources::IsExisting(guid))return Resources::Load(guid);
        }

        //加载导入器
        const ObjectPtr<Importer> importer = LoadImporter(path);
        if (importer == nullptr) return {};
        const Guid guid = importer->GetGuid();
        //导入物体
        ObjectPtrBase object = importer->Import(rootDirectory + path);
        object->SetName(ParseFileName(path));
        //检查是否为重新导入
        if (const int lastInstance = ObjectGuid::GetInstanceID(guid); lastInstance != 0)
            Object::ReplaceObject(object, lastInstance);

        ObjectGuid::SetInstanceID(guid, object.GetInstanceID());
        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        Resources::Save(guid, object);

        return object;
    }

    void Assets::StaticConstructor()
    {
        ObjectSerializerBase::AddFindSerializationFallback([](const Guid& guid)
        {
            const std::string path = guidToPath[guid];
            if (std::filesystem::exists(path))
            {
                Load(path.substr(rootDirectory.size()));
                return Resources::ReadFile(guid);
            }

            return std::string{};
        });
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
                else if (item.path().extension() == ".importer")
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
    ObjectPtr<Importer> Assets::LoadImporter(const std::string& path)
    {
        const std::string lastRootPath = Resources::GetRootPath();
        Resources::SetRootPath(rootDirectory);

        auto jsonSerializer = Resources::CreateJsonSerializer();
        const std::string importerPath = path + ".importer";
        ObjectPtr<Importer> importer = Resources::IsExisting(importerPath) == false
                                           ? Importer::GetAssetsImporter(path.substr(path.find('.') + 1))
                                           : Resources::LoadOf<Importer>(importerPath, jsonSerializer);
        if (importer.IsNotNull())
            Resources::Save(importerPath, importer, jsonSerializer);

        Resources::SetRootPath(lastRootPath);
        return importer;
    }
}
