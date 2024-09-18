#include "Cursor.h"
namespace BDXKEngine
{
    void Cursor::SetLockState(bool state)
    {
        if (state)
        {
            isLocking = true;
            double posX, posY;
            glfwGetCursorPos(glfWwindow, &posX, &posY);
            lockingPos = Vector2{static_cast<float>(posX), static_cast<float>(posY)};
        }
        else
            isLocking = false;

        // if (state)
        //     glfwSetInputMode(glfWwindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        // else
        //     glfwSetInputMode(glfWwindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }

    void Cursor::SetVisible(bool state)
    {
        if (state)
            glfwSetInputMode(glfWwindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(glfWwindow,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    }

    void Cursor::Initialize(GLFWwindow* glfWwindow)
    {
        Cursor::glfWwindow = glfWwindow;
    }
    void Cursor::Update()
    {
        if (isLocking)
            glfwSetCursorPos(glfWwindow, static_cast<double>(lockingPos.x), static_cast<double>(lockingPos.y));
    }
}
