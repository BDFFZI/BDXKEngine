#include "Input.h"
#include "Cursor.h"

namespace BDXKEngine
{
    Vector2 Input::GetMouseMoveDeltaReal()
    {
        return mousePositionDelta;
    }
    Vector2 Input::GetMousePositionReal()
    {
        return mousePosition;
    }
    Vector2 Input::GetMouseScrollDeltaReal()
    {
        return mouseScrollDelta;
    }

    bool Input::GetMouseButtonDown(MouseButton mouseButton)
    {
        const int index = static_cast<int>(mouseButton);
        if (mouseButtonState[index][0] == false &&
            mouseButtonState[index][1] == true)
            return true;
        return false;
    }
    bool Input::GetMouseButton(MouseButton mouseButton)
    {
        const int index = static_cast<int>(mouseButton);
        if (mouseButtonState[index][0] == true &&
            mouseButtonState[index][1] == true)
            return true;
        return false;
    }
    bool Input::GetMouseButtonUp(MouseButton mouseButton)
    {
        const int index = static_cast<int>(mouseButton);
        if (mouseButtonState[index][0] == true &&
            mouseButtonState[index][1] == false)
            return true;
        return false;
    }
    bool Input::GetKeyDown(KeyCode keyCode)
    {
        if (keyboardState[static_cast<int>(keyCode)][0] == false &&
            keyboardState[static_cast<int>(keyCode)][1] == true)
            return true;
        return false;
    }
    bool Input::GetKey(KeyCode keyCode)
    {
        if (keyboardState[static_cast<int>(keyCode)][0] == true &&
            keyboardState[static_cast<int>(keyCode)][1] == true)
            return true;
        return false;
    }
    bool Input::GetKeyUp(KeyCode keyCode)
    {
        if (keyboardState[static_cast<int>(keyCode)][0] == true &&
            keyboardState[static_cast<int>(keyCode)][1] == false)
            return true;
        return false;
    }

    Vector2 Input::GetMouseMoveDelta()
    {
        if (enable == false)
            return Vector2::zero;

        return GetMouseMoveDeltaReal();
    }
    Vector2 Input::GetMousePosition()
    {
        if (enable == false)
            return Cursor::lockingPos;

        return GetMousePositionReal();
    }
    Vector2 Input::GetMouseScrollDelta()
    {
        if (enable == false)
            return Vector2::zero;

        return GetMouseScrollDeltaReal();
    }

    void Input::SetIsEnabling(bool state)
    {
        enable = state;
    }

    void Input::Initialize(GLFWwindow* glfWwindow)
    {
        Input::glfWwindow = glfWwindow;
        glfwSetScrollCallback(glfWwindow, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            mouseScrollDeltaNew = {static_cast<float>(xoffset), static_cast<float>(yoffset)};
        });
        glfwSetMouseButtonCallback(glfWwindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            if (enable == false)
                action = false;

            mouseButtonState[button][2] = action;
        });
        glfwSetKeyCallback(glfWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (enable == false)
                action = false;

            keyboardState[key][2] = action;
        });
    }
    void Input::Update()
    {
        //鼠标按钮
        for (auto& state : mouseButtonState)
        {
            state[0] = state[1];
            state[1] = state[2];
        }
        //键盘按钮
        for (auto& state : keyboardState)
        {
            state[0] = state[1];
            state[1] = state[2];
        }
        //滚轮增量
        mouseScrollDelta = mouseScrollDeltaNew;
        mouseScrollDeltaNew = Vector2::zero;
        //鼠标位置
        mousePositionLast = mousePosition;
        double posX, posY;
        glfwGetCursorPos(glfWwindow, &posX, &posY);
        mousePosition = Vector2{static_cast<float>(posX), static_cast<float>(posY)};
        mousePositionDelta = mousePosition - (Cursor::isLocking ? Cursor::lockingPos : mousePositionLast);
    }
}
