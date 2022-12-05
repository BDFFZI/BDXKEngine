#include "Texture2DImporter.h"

#include "BDXKEngine/Platform/GL/Texture/TextureImport.h"

namespace BDXKEditor
{
    ObjectPtrBase Texture2DImporter::Import(std::string filePath)
    {
        return static_cast<ObjectPtrBase>(TextureImport::Png(filePath));
    }
}
