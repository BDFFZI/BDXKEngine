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
        return Importer::GetAssetsImporter(path.substr(path.rfind('.') + 1)).IsNotNull();
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
        Resources::Save(guid, object); //便于下次直接用缓存文件加载
        return object;
    }
    void Assets::Save(const std::string& path, ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull())throw std::exception("目标物体不存在");

        if (pathToGuid.contains(path))
        {
            const Guid guid = pathToGuid[path];
            const int instanceID = ObjectGuid::GetInstanceID(guid);
            if (objectPtr.GetInstanceID() != instanceID)
            {
                //检测到覆盖保存，Guid必须锁定为和导入器一致（首次导入时导入器能够负责这一块，但无法保证后续修改的问题）
                //虽然导入器导入时会纠正Guid，但无法纠正在这之前发生的反序列化阶段的Guid
                //所以覆盖资源时必须保证和导入器Guid一致
                if (instanceID != 0)Object::ReplaceObject(objectPtr, instanceID); //替换旧物体
                else ObjectGuid::SetInstanceID(guid, objectPtr.GetInstanceID()); //旧物体并未被加载
            }
        }

        auto serializer = Serialization::CreateJsonSerializer();
        Serialization::Save(rootDirectory + path, objectPtr, serializer);
        Resources::Save(ObjectGuid::GetOrSetGuid(objectPtr.GetInstanceID()), objectPtr); //更新或创建缓存文件
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
    ObjectPtr<Importer> Assets::LoadImporter(const std::string& path)
    {
        auto jsonSerializer = Serialization::CreateJsonSerializer();
        const std::string importerPath = rootDirectory + path + ".importer";
        ObjectPtr<Importer> importer = Serialization::IsExisting(importerPath) == false
                                           ? Importer::GetAssetsImporter(path.substr(path.rfind('.') + 1))
                                           : Serialization::Load<Importer>(importerPath, jsonSerializer);
        if (importer.IsNull())throw std::exception("无法加载目标对应的导入器");

        if (importer->GetTargetGuid().empty())importer->Import(rootDirectory + path); //新创建的importer保存前必须要先用一次（从而获得资源的Guid）
        Serialization::Save(importerPath, importer, jsonSerializer);
        return importer;
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
