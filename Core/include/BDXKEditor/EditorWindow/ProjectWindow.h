#pragma once
#include <filesystem>
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"

namespace BDXKEditor
{
    class ProjectWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);
    private:
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;

        void ShowDirectory(const std::filesystem::directory_entry& directoryEntry);
        void OnGUI() override;
    };
}
