#pragma once
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"

namespace BDXKEditor
{
    class ConsoleWindow : public EditorWindow,public AwakeHandler
    {
    public:
        static std::stringstream& GetConsole();
    private:
        void OnGUI() override;
        void OnAwake() override;
    };
}
