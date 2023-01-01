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

        //加载物体
        const ObjectPtr<Importer> importer = LoadImporter(path);
        const ObjectPtrBase object = importer->Import(rootDirectory + path);
        const Guid guid = importer->GetTargetGuid();

        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        Resources::Save(guid, object);
        return object;
    }
    void Assets::Save(const std::string& path, const ObjectPtrBase& objectPtr)
    {
        auto serializer = Serialization::CreateJsonSerializer();
        return Serialization::Save(rootDirectory + path, objectPtr, serializer);
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
        auto jsonSerializer = Serialization::CreateJsonSerializer();
        const std::string importerPath = rootDirectory + path + ".importer";
        ObjectPtr<Importer> importer = Serialization::IsExisting(importerPath) == false
                                           ? Importer::GetAssetsImporter(path.substr(path.find('.') + 1))
                                           : Serialization::Load<Importer>(importerPath, jsonSerializer);
        if (importer.IsNull())throw std::exception("无法加载目标对应的导入器");

        Serialization::Save(importerPath, importer, jsonSerializer);
        return importer;
    }

    void Assets::StaticConstructor()
    {
        if (std::filesystem::exists("Assets") == false)
            std::filesystem::create_directory("Assets");
        
        ObjectSerializerBase::AddFindSerializationFallback([](const Guid& guid)
        {
            if (guidToPath.contains(guid))
            {
                const std::string path = rootDirectory + guidToPath[guid];
                if (std::filesystem::exists(path))
                {
                    Load(path.substr(rootDirectory.size()));
                    return Resources::FindSerializationFallback(guid);
                }
            }

            return static_cast<Reflective*>(nullptr);
        });
    }
}
