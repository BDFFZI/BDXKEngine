#pragma once
#include <functional>
#include <vector>
#include <windows.h>
#include "BDXKEngine/Base/Data/Graphics/Rect.h"

namespace BDXKEngine
{
    enum class KeyCode :int
    {
        Backspace = 0x08,
        Delete = 0x2E,
        Tab = 0x09,
        Return = 0x0D,
        Esc = 0x1B,
        Space = 0x20,
        Alpha0 = 0x30,
        Alpha1 = 0x31,
        Alpha2 = 0x32,
        Alpha3 = 0x33,
        Alpha4 = 0x34,
        Alpha5 = 0x35,
        Alpha6 = 0x36,
        Alpha7 = 0x37,
        Alpha8 = 0x38,
        Alpha9 = 0x39,
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4A,
        K = 0x4B,
        L = 0x4C,
        M = 0x4D,
        N = 0x4E,
        O = 0x4F,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5A,
        Shift = 0x10,
    };

    typedef std::function<void()> RenewEvent;
    typedef std::function<void(Vector2 size)> ResizeEvent;
    typedef std::function<void()> DestroyEvent;
    typedef std::function<void(Vector2 position)> MouseMoveEvent;
    typedef std::function<void(Vector2 delta)> MouseWheelEvent;
    typedef std::function<void(int button, bool state)> MouseButtonEvent;
    typedef std::function<void(KeyCode code, bool state)> KeyCodeEvent;
    typedef std::function<void(char character)> CharacterEvent;
    typedef std::function<void(HWND, UINT, WPARAM, LPARAM)> NativeEvent;

    class Window
    {
    public:
        static void Initialize(const wchar_t* name);

        static std::wstring GetName();
        static HWND GetHwnd();
        static Vector2 GetSize();
        static Vector2 GetCursorMoveDelta();
        static void SetCursorLock(bool state);
        static void SetCursorVisible(bool state);
        static void AddRenewEvent(const RenewEvent& renewEvent);
        static void AddResizeEvent(const ResizeEvent& resizeEvent);
        static void AddDestroyEvent(const DestroyEvent& destroyEvent);
        static void AddMouseMoveEvent(const MouseMoveEvent& mouseMoveEvent);
        static void AddMouseWheelEvent(const MouseWheelEvent& mouseWheelEvent);
        static void AddMouseButtonEvent(const MouseButtonEvent& mouseButtonEvent);
        static void AddKeyCodeEvent(const KeyCodeEvent& keyCodeEvent);
        static void AddCharacterEvent(const CharacterEvent& characterEvent);
        static void AddNativeEvent(const NativeEvent& characterEvent);
        static void Show(const std::function<bool()>& isOver = [] { return false; });

        static Rect GetScreenRect(); //获取当前窗口在屏幕中的范围
        static Vector2 GetCursorLocalPosition(); //获取当前光标相对窗口的位置
        static void SetCursorLocalPosition(Vector2 localPosition); //设置当前光标相对窗口的位置
        static void SetCursorConfining(bool state); //将光标限制在窗口内
    private:
        static PCWSTR name;
        static HWND hwnd;
        static HCURSOR hCursor;
        static std::vector<RenewEvent> renewEvents;
        static std::vector<ResizeEvent> resizeEvents;
        static std::vector<DestroyEvent> destroyEvents;
        static std::vector<MouseMoveEvent> mouseMoveEvents;
        static std::vector<MouseWheelEvent> mouseWheelEvents;
        static std::vector<MouseButtonEvent> mouseButtonEvents;
        static std::vector<KeyCodeEvent> keyCodeEvents;
        static std::vector<CharacterEvent> characterEvents;
        static std::vector<NativeEvent> nativeEvents;
        static bool cursorlock;
        static bool cursorVisible;
        static Vector2 lockCursorPos;
        static Vector2 lastCursorPos;
        static Vector2 cursorPos;
        static int pressedMouse; //当前按下的鼠标按钮数量

        static void SetCursorTrack(bool state); //当光标超出窗口范围后是否继续跟踪鼠标状态
        static void UpdateCursor(); //更新光标显示效果

        static LRESULT WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
    };
}
