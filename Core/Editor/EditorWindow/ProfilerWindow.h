#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"

namespace BDXKEditor
{
    class ProfilerWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);
    private:
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;

        void DrawObjects() const;
        void DrawGuids() const;
        void OnGUI() override;
    };
    CustomReflection(ProfilerWindow)
}
