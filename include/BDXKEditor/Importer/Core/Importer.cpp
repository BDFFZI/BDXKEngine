#include "Importer.h"

#include "BDXKEngine/Base/Object/Serialization/ObjectGuid.h"

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

    const Guid& Importer::GetGuid()
    {
        return guid;
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
