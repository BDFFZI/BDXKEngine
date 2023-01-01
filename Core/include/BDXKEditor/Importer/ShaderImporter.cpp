#include "ShaderImporter.h"

#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"

namespace BDXKEditor
{
    ObjectPtrBase ShaderImporter::ImportObject(std::string filePath)
    {
        return static_cast<ObjectPtrBase>(ShaderImport::Hlsl(filePath));
    }
}
