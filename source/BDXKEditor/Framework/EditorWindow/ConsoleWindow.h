#pragma once
#include "Core/EditorWindow.h"

namespace BDXKEditor
{
    class ConsoleWindow : public EditorWindow, public AwakeHandler
    {
    public:
        static std::stringstream& GetConsole();
    private:
        inline static std::stringstream stringstream = {};

        void OnGUI() override;
        void OnAwake() override;

        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
