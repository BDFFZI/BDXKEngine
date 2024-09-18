#pragma once
#include <filesystem>
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Runtime/Base/Object/Object.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;
    class PackageSetting : public Object
    {
    public:
        static std::filesystem::path GetAssetsPath(const std::string& packageName);


        void Transfer(Transferer& transferer) override;

    private:
        inline static std::vector<std::string> packageNames = {};
        inline static std::vector<std::string> packagePaths = {};

        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, -15)
    };
}
