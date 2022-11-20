#pragma once
#include "Shader.h"

namespace BDXKEngine
{
    class ShaderImporter
    {
    public:
        static ObjectPtr<Shader> Import(const std::string& fileName);
    };
}
