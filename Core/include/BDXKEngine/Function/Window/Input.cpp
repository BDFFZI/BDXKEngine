#include "Input.h"

namespace BDXKEngine
{
    Vector2 Input::mouseScrollDelta;
    Vector2 Input::mousePosition;
    bool Input::mouseButtonState[3][3];
    bool Input::keyboardState[256][3];
    bool Input::enable = true;

    bool Input::GetMouseButtonDown(int mouseButtonIndex)
    {
        if (mouseButtonState[mouseButtonIndex][0] == false &&
            mouseButtonState[mouseButtonIndex][1] == true)
            return true;
        return false;
    }
    bool Input::GetMouseButton(int mouseButtonIndex)
    {
        if (mouseButtonState[mouseButtonIndex][0] == true &&
            mouseButtonState[mouseButtonIndex][1] == true)
            return true;
        return false;
    }
    bool Input::GetMouseButtonUp(int mouseButtonIndex)
    {
        if (mouseButtonState[mouseButtonIndex][0] == true &&
            mouseButtonState[mouseButtonIndex][1] == false)
            return true;
        return false;
    }

    Vector2 Input::GetMouseMoveDelta()
    {
        if (enable == false)
            return Vector2::zero;

        return Window::GetCursorMoveDelta();
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
    void Input::SetIsEnabling(bool state)
    {
        enable = state;
    }

    void Input::Initialize()
    {
        Window::AddRenewEvent([]()
        {
            mouseScrollDelta = Vector2::zero;

            for (auto& state : mouseButtonState)
            {
                state[0] = state[1];
                state[1] = state[2];
            }
            for (auto& state : keyboardState)
            {
                state[0] = state[1];
                state[1] = state[2];
            }
        });
        Window::AddMouseMoveEvent([](Vector2 position)
        {
            if (enable == false)
                position = mousePosition;

            mousePosition = position;
        });
        Window::AddMouseWheelEvent([](Vector2 delta)
        {
            if (enable == false)
                delta = Vector2::zero;

            mouseScrollDelta = delta;
        });
        Window::AddMouseButtonEvent([](int mouseButton, bool state)
        {
            if (enable == false)
                state = false;

            mouseButtonState[mouseButton][2] = state;
        });
        Window::AddKeyCodeEvent([](KeyCode keyCode, bool state)
        {
            if (enable == false)
                state = false;

            keyboardState[static_cast<int>(keyCode)][2] = state;
        });
    }
}
