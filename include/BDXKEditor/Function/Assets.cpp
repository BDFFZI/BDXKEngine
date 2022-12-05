#include "Assets.h"
#include <filesystem>
#include <fstream>
#include "BDXKEngine/Platform/Resources/Resources.h"

namespace BDXKEditor
{
    const std::string rootDirectory = "Assets/";

    std::string Assets::JsonSerializerAdapter::LoadSerialization(const Guid& guid)
    {
        return ReadFile(guidToPath[guid]);
    }

    std::string Assets::GetRootPath()
    {
        return rootDirectory.substr(0, rootDirectory.size() - 1);
    }

    ObjectPtrBase Assets::Load(const std::string& path)
    {
        if (pathToGuid.contains(path))
        {
            const Guid guid = pathToGuid[path];
            const int instance = ObjectGuid::GetInstanceID(guid);
            if (instance != 0)return Object::FindObjectOfInstanceID(instance);
            if (Resources::IsExisting(guid))return Resources::Load(guid);
        }

        const ObjectPtr<Importer> importer = LoadImporter(path);
        if (importer == nullptr) return {};
        ObjectPtrBase object = importer->Import(rootDirectory + path);
        object->SetName(ParseFileName(path));

        const Guid guid = ObjectGuid::GetOrSetGuid(object.GetInstanceID());
        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        Resources::Save(guid, object);

        return object;
    }
    ObjectPtr<Importer> Assets::LoadImporter(const std::string& path)
    {
        auto jsonSerializer = Resources::CreateJsonSerializer();
        jsonSerializer.SetAdapter(JsonSerializerAdapter{});

        const std::string lastRootPath = Resources::GetRootPath();
        Resources::SetRootPath(rootDirectory);

        const std::string importerPath = path + ".importer";
        ObjectPtr<Importer> importer = Resources::IsExisting(importerPath) == false
                                           ? Importer::GetAssetsImporter(path.substr(path.find('.') + 1))
                                           : Resources::LoadOf<Importer>(importerPath, jsonSerializer);
        if (importer.IsNotNull())
            Resources::Save(importerPath, importer, jsonSerializer);

        Resources::SetRootPath(lastRootPath);

        return importer;
    }

    void Assets::StaticConstructor()
    {
        std::string json = ReadFile("assets.json");
        if (json.empty())return;

        JsonImporter importer = {};
        importer.Reset(json);

        int count = static_cast<int>(pathToGuid.size());
        importer.TransferField("Count", count);

        for (int index = 0; index < count; index++)
        {
            std::string path;
            std::string guid;

            std::string layer = "Item_" + std::to_string(index);
            importer.PushPath(layer);
            importer.TransferField("Path", path);
            importer.TransferField("Guid", guid);
            importer.PopPath(layer);

            pathToGuid[path] = guid;
            guidToPath[guid] = path;
        }
    }
    void Assets::StaticDestructor()
    {
        JsonExporter exporter = {};

        int count = static_cast<int>(pathToGuid.size());
        exporter.TransferField("Count", count);

        int index = 0;
        for (const auto& item : pathToGuid)
        {
            std::string path = item.first;
            std::string guid = item.second;

            std::string layer = "Item_" + std::to_string(index);
            exporter.PushPath(layer);
            exporter.TransferField("Path", path);
            exporter.TransferField("Guid", guid);
            exporter.PopPath(layer);
            index++;
        }

        std::string json;
        exporter.Reset(json);
        std::ofstream ofstream = std::ofstream{"assets.json", std::fstream::binary};
        ofstream << json;
        ofstream.close();
    }
}
