#pragma once
#include <filesystem>
#include "Core/EditorWindow.h"

namespace BDXKEditor
{
    class ProjectWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);
    private:
        inline static std::unordered_map<std::string, bool> isUnfolding = {};
        static void StaticConstructor();
        static void StaticDestructor();
        CustomStaticConstructor(StaticConstructor)
        CustomStaticDestructor(StaticDestructor)

        std::function<void(const ObjectPtrBase&)> clickObjectEvent;

        void ShowDirectory(const std::filesystem::path& directoryEntry);
        void OnGUI() override;
    };
}
