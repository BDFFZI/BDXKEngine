#include "Importer.h"
#include "BDXKEngine/Base/Object/Serializer/ObjectGuid.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"

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
    std::string ParseFileExtension(const std::string& path)
    {
        const size_t index = path.rfind('.');
        if (index == std::string::npos)
            return {};
        return path.substr(index + 1);
    }

    ObjectPtr<Importer> Importer::GetAssetsImporter(const std::string& path)
    {
        const std::string extension = ParseFileExtension(path);
        if (assetImporter.contains(extension) == false)
            throw std::exception("不存在适用于目标物体的导入器");

        auto jsonSerializer = Serialization::CreateJsonSerializer();
        const std::string porterPath = path + ".importer";
        return Serialization::IsExisting(porterPath)
                   ? Serialization::Load<Importer>(porterPath, jsonSerializer)
                   : Create(assetImporter[extension]).ToObject<Importer>();
    }
    void Importer::SetAssetsImporter(const std::string& extension, const Type& type)
    {
        assetImporter[extension] = type;
    }
    bool Importer::HasAssetsImporter(const std::string& extension)
    {
        return assetImporter.contains(extension);
    }

    const Guid& Importer::GetTargetGuid()
    {
        return guid;
    }

    ObjectPtrBase Importer::Import(const std::string& filePath)
    {
        ObjectPtrBase object = ImportAsset(filePath);
        RectifyGuid(object);

        auto serializer = Serialization::CreateJsonSerializer();
        Serialization::Save(filePath + ".importer", this, serializer);
        return object;
    }
    void Importer::Export(const std::string& filePath, ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull())
            throw std::exception("目标物体不存在");

        RectifyGuid(objectPtr);
        ExportAsset(filePath, objectPtr);

        auto serializer = Serialization::CreateJsonSerializer();
        Serialization::Save(filePath + ".importer", this, serializer);
    }

    void Importer::ExportAsset(const std::string& filePath, const ObjectPtrBase& objectPtr)
    {
        auto serializer = Serialization::CreateJsonSerializer();
        Serialization::Save(filePath, objectPtr, serializer);
    }

    void Importer::RectifyGuid(ObjectPtrBase& objectPtr)
    {
        if (guid.empty())
        {
            //首次使用，导出器将使用Object提供的Guid
            guid = ObjectGuid::GetOrSetGuid(objectPtr.GetInstanceID());
            return;
        }

        //重复使用
        if (const ID instanceID = ObjectGuid::GetInstanceID(guid); objectPtr.GetInstanceID() != instanceID)
        {
            //一个新创建的物体或其他位置导入的资源想要覆盖保存，Guid必须锁定为和导入器一致
            if (instanceID == 0)
                ObjectGuid::SetInstanceID(guid, objectPtr.GetInstanceID()); //原物体未被加载,不需要替换，直接绑定instanceID和Guid即可
            else
                ReplaceObject(objectPtr, instanceID); //内存中存在旧物体，需要替换
        }
    }
    void Importer::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(guid);
    }
}
