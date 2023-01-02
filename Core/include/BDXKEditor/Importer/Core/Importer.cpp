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
        ObjectPtrBase object = ImportAsset(filePath);
        //检查是否为重新导入(以导入器中的Guid为准，因为有些资源自身不带Guid)
        if (ObjectGuid::GetInstanceID(guid) != object.GetInstanceID()) //资源自带Guid时序列化器会自动处理，所以不需要再次处理
        {
            if (ObjectGuid::GetInstanceID(guid) == 0)
                ObjectGuid::SetInstanceID(guid, object.GetInstanceID());
            else
                ReplaceObject(object, ObjectGuid::GetInstanceID(guid));
        }

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
