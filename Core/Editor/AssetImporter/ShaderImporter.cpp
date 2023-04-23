#include "ShaderImporter.h"

#include "Core/Runtime/Platform/GL/Shader/Shader.h"

namespace BDXKEditor
{
    ObjectPtrBase ShaderImporter::Import()
    {
        ObjectPtrBase objectPtrBase = Shader::CreateFromFile(GetTargetPath(), false).ToObject();
        BindObjectRef(objectPtrBase, 1000000001);
        return objectPtrBase;
    }
}
