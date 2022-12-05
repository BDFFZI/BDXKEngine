#include "Importer.h"

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
}
