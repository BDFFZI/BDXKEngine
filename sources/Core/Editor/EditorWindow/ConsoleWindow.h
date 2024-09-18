#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"

namespace BDXKEditor
{
    class ConsoleWindow : public EditorWindow, public AwakeEvent
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
    CustomReflection(ConsoleWindow)
}
