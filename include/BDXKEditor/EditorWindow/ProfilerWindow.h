#pragma once
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"

namespace BDXKEditor
{
    class ProfilerWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);
    private:
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;
        
        void OnGUI() override;
    };
}
