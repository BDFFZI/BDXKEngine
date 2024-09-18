#pragma once
#include <GLFW/glfw3.h>
#include "Core/Runtime/Base/Data/Math/Vector/Vector2.h"

namespace BDXKEngine
{
    class Window;

    class Screen
    {
        friend Window;
    public:
        static Vector2 GetSize();
    private:
        inline static GLFWwindow* glfWwindow;

        static void Initialize(GLFWwindow* glfWwindow);
    };
}
