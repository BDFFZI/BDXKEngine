#include "AssetDatabase.h"
#include <filesystem>
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Framework/Scene/SceneLibrary.h"

namespace BDXKEditor
{
    std::filesystem::path AssetDatabase::GetAssetPath(const ObjectPtrBase& objectPtr)
    {
        const ObjectRef objectRef = ObjectRef::GetObjectRef(objectPtr.GetObjectID());
        return guidToPath.contains(objectRef.guid) ? guidToPath.at(objectRef.guid) : "";
    }
    bool AssetDatabase::IsCanImport(const std::filesystem::path& path)
    {
        return AssetImporter::HasAssetsImporter(path.extension());
    }

    ObjectPtrBase AssetDatabase::Load(const std::filesystem::path& path)
    {
        if (pathToGuid.contains(path) == false)
            return ReLoad(path);

        const xg::Guid guid = pathToGuid[path];
        const ID instance = ObjectRef::GetRootObjectID(guid);
        if (const Object* object = Object::FindObjectOfObjectID(instance); object != nullptr)
            return object; //资源已经在内存中
        if (SceneLibrary::IsExisting(guid))
            return SceneLibrary::Load(guid);

        Debug::LogError("资源加载失败,请尝试重新导入。");
        return {};
    }
    ObjectPtrBase AssetDatabase::ReLoad(const std::filesystem::path& path)
    {
        ObjectPtrBase objectPtr = Import(path);
        Object::AwakeObject(objectPtr);
        return objectPtr;
    }
    void AssetDatabase::Save(const std::filesystem::path& path, const ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull())
            throw std::exception("目标物体不存在");

        //获取导入器
        const ObjectPtr<AssetImporter> importer = AssetImporter::GetAtPath(path);
        const xg::Guid guid = importer->GetTargetGuid();
        //导出资源
        auto serializer = ObjectSerialization::CreateJsonSerializer();
        ObjectSerialization::Save(path, objectPtr, serializer, guid);
        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        //另存为运行时资源
        SceneLibrary::Save(guid, objectPtr);
    }

    ObjectPtrBase AssetDatabase::Import(const std::filesystem::path& path)
    {
        //获取导入器
        const ObjectPtr<AssetImporter> importer = AssetImporter::GetAtPath(path);
        const xg::Guid guid = importer->GetTargetGuid();
        //导入素材
        const ObjectPtrBase object = importer->ImportAsset();
        pathToGuid[path] = guid;
        guidToPath[guid] = path;
        //另存为运行时资源
        SceneLibrary::Save(guid, object);
        return object;
    }

    void AssetDatabase::AddRefreshDir(const std::string& name, const std::filesystem::path& path)
    {
        if (exists(path) == false)
            create_directory(path);
        refreshDirs.insert(refreshDirs.begin(), std::make_tuple(name, path));
    }
    const std::vector<std::tuple<std::string, std::filesystem::path>>& AssetDatabase::GetAllRefreshDirs()
    {
        return refreshDirs;
    }
    void AssetDatabase::Refresh()
    {
        pathToGuid.clear();
        guidToPath.clear();

        std::unordered_map<std::filesystem::path, int> isImported;

        //遍历所有目录
        std::stack<std::filesystem::directory_entry> directories = {};
        for (auto& [name,path] : refreshDirs)
            directories.push(std::filesystem::directory_entry{path});

        while (directories.empty() == false)
        {
            //遍历当前目录
            auto iterator = std::filesystem::directory_iterator{directories.top()};
            directories.pop();
            for (const auto& item : iterator)
            {
                //是目录就入栈待会再迭代
                if (item.is_directory())
                    directories.push(item);
                else
                {
                    std::string file = item.path().string();
                    if (item.path().extension() == ".importer")
                        isImported[file.substr(0, file.size() - 9)] ++;
                    else
                        isImported[file] ++;
                }
            }
        }

        for (auto& [assetPath,count] : isImported)
        {
            if (AssetImporter::HasAssetsImporter(assetPath.extension()) == false)
                continue;
            if (count == 1) //需要创建导入器
                AssetImporter::GetAtPath(assetPath);

            //获取guid
            std::filesystem::path importPath = assetPath;
            importPath.concat(".importer");
            rapidjson::Document document = {};
            document.Parse(ReadFile(importPath).c_str());
            xg::Guid guid = xg::Guid{document["object_0"]["data"]["guid"].GetString()};
            //绑定guid和path的关系
            pathToGuid[assetPath] = guid;
            guidToPath[guid] = assetPath;
        }

        //缓存运行时数据，必须在统计guid和path之后执行，不然会出现引用丢失
        for (const auto& [guid,path] : guidToPath)
        {
            if (SceneLibrary::IsExisting(guid) == false)
                Import(path);
        }
    }

    void AssetDatabase::OnEngineStart()
    {
        ObjectSerialization::AddDeserializeFallback([](const xg::Guid guid)
        {
            if (guidToPath.contains(guid))
                return Import(guidToPath.at(guid));
            return ObjectPtrBase{};
        });

        AddRefreshDir("Assets", "Assets");
        Refresh();
    }
}
