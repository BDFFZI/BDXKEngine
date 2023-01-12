#include "Importer.h"
#include "BDXKEngine/Base/Object/Serializer/ObjectGuid.h"

namespace BDXKEditor
{
    std::string ParseFileName(const std::string& path)
    {
        size_t index = path.rfind('\\');
        if (index == std::string::npos)
            index = path.rfind('/');
        if (index == std::string::npos)
            return path;

        return path.substr(index + 1);
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
        if (guid.empty())guid = ObjectGuid::GetOrSetGuid(object.GetInstanceID()); //首次导入，必须保证导入器提供的Guid和Object的Guid一致

        //检查是否为重新导入(以导入器中的Guid为准,因为有些资源自身不带Guid，若带有Guid也应该和导入器中的Guid一致)
        if (ObjectGuid::GetInstanceID(guid) != object.GetInstanceID())
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
}
