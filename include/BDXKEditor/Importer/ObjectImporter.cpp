#include "ObjectImporter.h"

#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEditor
{
    ObjectPtrBase ObjectImporter::ImportObject(std::string filePath)
    {
        auto serializer = Serialization::CreateJsonSerializer();
        return Serialization::Load(filePath, serializer);
    }
}
