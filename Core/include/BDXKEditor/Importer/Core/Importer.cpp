#include "Importer.h"

#include "BDXKEngine/Base/Object/Serializer/ObjectGuid.h"

namespace BDXKEditor
{
    std::string ParseFileName(const std::string& path)
    {
        return path.substr(path.rfind('\\') + 1);
    }

    ObjectPtr<Importer> Importer::GetAssetsImporter(const std::string& extension)
    {
        if (assetsImporter.contains(extension) == false)
            return {};

        return Create(assetsImporter[extension]).ToObject<Importer>();
    }
    void Importer::SetAssetsImporter(const std::string& extension, const Type& type)
    {
        assetsImporter[extension] = type;
    }

    const Guid& Importer::GetTargetGuid()
    {
        return guid;
    }
    ObjectPtrBase Importer::Import(const std::string& filePath)
    {
        //导入物体
        ObjectPtrBase object = ImportObject(filePath);
        object->SetName(ParseFileName(filePath));
        //检查是否为重新导入
        if (const int lastInstance = ObjectGuid::GetInstanceID(guid); lastInstance != 0)
            ReplaceObject(object, lastInstance);
        //设置Guid关联信息
        ObjectGuid::SetInstanceID(guid, object.GetInstanceID());

        return object;
    }

    void Importer::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(guid);
    }
    void Importer::Awake()
    {
        Object::Awake();

        if (guid.empty())
            guid = NewGuid();
    }
}
