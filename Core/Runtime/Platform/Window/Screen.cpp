#include "Screen.h"

namespace BDXKEngine
{
    void Screen::Initialize(GLFWwindow* glfWwindow)
    {
        Screen::glfWwindow = glfWwindow;
    }

    Vector2 Screen::GetSize()
    {
        int width;
        int height;
        glfwGetWindowSize(glfWwindow, &width, &height);
        return {width, height};
    }
}
