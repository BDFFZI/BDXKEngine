#pragma once
#include "Shader.h"

namespace BDXKEngine
{
    class ShaderImport
    {
    public:
        static ObjectPtr<Shader> Hlsl(const std::string& fileName);
    };
}
