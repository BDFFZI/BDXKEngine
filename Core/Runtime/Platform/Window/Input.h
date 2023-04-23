#pragma once
#include <GLFW/glfw3.h>
#include "Core/Runtime/Base/Data/Math/Vector/Vector2.h"

namespace BDXKEngine
{
    enum class KeyCode
    {
        Backspace = GLFW_KEY_BACKSPACE,
        Delete = GLFW_KEY_DELETE,
        Tab = GLFW_KEY_TAB,
        Return = GLFW_KEY_ENTER,
        Esc = GLFW_KEY_ESCAPE,
        Space = GLFW_KEY_SPACE,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        Alpha0 = GLFW_KEY_0,
        Alpha1 = GLFW_KEY_1,
        Alpha2 = GLFW_KEY_2,
        Alpha3 = GLFW_KEY_3,
        Alpha4 = GLFW_KEY_4,
        Alpha5 = GLFW_KEY_5,
        Alpha6 = GLFW_KEY_6,
        Alpha7 = GLFW_KEY_7,
        Alpha8 = GLFW_KEY_8,
        Alpha9 = GLFW_KEY_9,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
    };

    enum class MouseButton
    {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    };

    class Window;

    class Input
    {
        friend Window;

    public:
        static Vector2 GetMouseMoveDeltaReal();
        static Vector2 GetMousePositionReal();
        static Vector2 GetMouseScrollDeltaReal();

        static bool GetMouseButtonDown(MouseButton mouseButton);
        static bool GetMouseButton(MouseButton mouseButton);
        static bool GetMouseButtonUp(MouseButton mouseButton);
        static bool GetKeyDown(KeyCode keyCode);
        static bool GetKey(KeyCode keyCode);
        static bool GetKeyUp(KeyCode keyCode);
        
        static Vector2 GetMouseMoveDelta();
        static Vector2 GetMousePosition();
        static Vector2 GetMouseScrollDelta();

        static void SetIsEnabling(bool state);

    private:
        inline static Vector2 mouseScrollDelta;
        inline static Vector2 mouseScrollDeltaNew;
        inline static Vector2 mousePositionLast;
        inline static Vector2 mousePosition;
        inline static Vector2 mousePositionDelta;
        inline static bool mouseButtonState[3][3];
        inline static bool keyboardState[256][3];
        inline static bool enable = true;

        inline static GLFWwindow* glfWwindow;

        static void Initialize(GLFWwindow* glfWwindow);
        static void Update();
    };
}
