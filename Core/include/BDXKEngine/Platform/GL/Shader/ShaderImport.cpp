#include "ShaderImport.h"
#include <filesystem>

namespace BDXKEngine
{
    ObjectPtr<Shader> ShaderImport::Hlsl(const std::string& fileName)
    {
        return Shader::CreateFromFile(fileName);
    }
}
