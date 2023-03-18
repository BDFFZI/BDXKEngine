#pragma once
#include <string>
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Reflection/Reflective.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class ProjectSettings
    {
    public:
        static std::string ParsePath(const std::string& fileName);

        static Reflective* Load(const std::string& fileName);
        template <typename TSettings>
        static TSettings* Load(const std::string& path)
        {
            return dynamic_cast<TSettings*>(Load(path));
        }
        static void Save(const std::string& fileName, Reflective* settings);

    private:
        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
