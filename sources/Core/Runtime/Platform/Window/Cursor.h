#pragma once
#include <GLFW/glfw3.h>
#include "Core/Runtime/Base/Data/Math/Vector/Vector2.h"

namespace BDXKEngine
{
    class Window;
    class Input;

    class Cursor
    {
        friend Window;
        friend Input;

    public:
        static void SetLockState(bool state);
        static void SetVisible(bool state);

    private:
        inline static GLFWwindow* glfWwindow;
        inline static bool isLocking = false;
        inline static Vector2 lockingPos;

        static void Initialize(GLFWwindow* glfWwindow);
        static void Update();
    };
}
