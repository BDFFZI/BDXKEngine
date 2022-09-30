#include "Window.h"
#include <cmath>

#include "Base/Extension/Debug.h"

namespace BDXKEngine
{
    std::unordered_set<Window*> Window::windows = {};

    void Window::Run()
    {
        while (windows.empty() == false)
        {
            MSG msg = {};
            GetMessage(&msg, nullptr, 0, 0);
            //预处理后交给窗口过程响应
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            //如果没有要处理的消息，我们就用这段空闲时间更新游戏
            if (PeekMessage(&msg, nullptr, NULL, NULL, NULL) == FALSE)
                PostMessage(msg.hwnd, WM_PAINT, NULL, NULL);
        }
    }

    Window::Window(const wchar_t* name, const Window* parent)
    {
        this->name = name;

        //向操作系统注册窗口类
        WNDCLASS windowClass = {};
        windowClass.lpszClassName = name; //类名，标识
        windowClass.lpfnWndProc = WindowProcess; //窗口消息处理函数
        RegisterClass(&windowClass);

        //创建窗口
        hwnd = CreateWindowEx(
            0, //窗口行为
            name, //窗口类名称
            name, //窗口标题
            WS_OVERLAPPEDWINDOW, //窗口样式
            CW_USEDEFAULT, CW_USEDEFAULT, //窗口位置xy
            CW_USEDEFAULT, CW_USEDEFAULT, //窗口大小xy
            parent == nullptr ? nullptr : parent->GetHwnd(), //父窗口
            nullptr, //窗口菜单
            nullptr, //实例句柄,默认
            this //附加的数据
        );

        cursorPos = GetCursorLocalPosition();
        lastCursorPos = cursorPos;
    }

    PCWSTR Window::GetName() const
    {
        return name;
    }
    HWND Window::GetHwnd() const
    {
        return hwnd;
    }
    Rect Window::GetScreenRect() const
    {
        POINT min{};
        POINT max = GetSize();
        ClientToScreen(GetHwnd(), &min);
        ClientToScreen(GetHwnd(), &max);

        return Rect{min, max};
    }
    Vector2 Window::GetSize() const
    {
        RECT rect;
        GetClientRect(GetHwnd(), &rect);
        return {static_cast<float>(rect.right), static_cast<float>(rect.bottom)};
    }
    Vector2 Window::GetCursorLocalPosition() const
    {
        Rect rect = GetScreenRect();

        POINT point;
        GetCursorPos(&point);

        return static_cast<Vector2>(point) - rect.GetMin();
    }
    Vector2 Window::GetCursorMoveDelta()
    {
        return cursorPos - lastCursorPos;
    }
    void Window::SetCursorTrack(bool state) const
    {
        if (state)
            SetCapture(GetHwnd());
        else
            ReleaseCapture();
    }
    void Window::SetCursorConfining(bool state) const
    {
        if (state)
        {
            const RECT rect = GetScreenRect();
            ClipCursor(&rect);
        }
        else
        {
            ClipCursor(nullptr);
        }
    }
    void Window::SetCursorLock(bool state)
    {
        SetCursorLocalPosition(GetSize() / 2);
        lockCursorPos = GetCursorLocalPosition();
        lastCursorPos = lockCursorPos;
        cursorPos = lockCursorPos;
        cursorlock = state;
    }
    void Window::SetCursorVisible(bool state)
    {
        cursorVisible = state;
        UpdateCursor();
    }
    void Window::SetCursorLocalPosition(Vector2 localPosition) const
    {
        Rect rect = GetScreenRect();
        const Vector2 position = rect.GetMin() + localPosition;

        SetCursorPos(static_cast<int>(std::round(position.x)), static_cast<int>(std::round(position.y)));
    }
    void Window::AddRenewEvent(const RenewEvent& renewEvent)
    {
        renewEvents.push_back(renewEvent);
    }
    void Window::AddResizeEvent(const ResizeEvent& resizeEvent)
    {
        resizeEvents.push_back(resizeEvent);
    }
    void Window::AddDestroyEvent(const DestroyEvent& destroyEvent)
    {
        destroyEvents.push_back(destroyEvent);
    }
    void Window::AddMouseMoveEvent(const MouseMoveEvent& mouseMoveEvent)
    {
        mouseMoveEvents.push_back(mouseMoveEvent);
    }
    void Window::AddMouseWheelEvent(const MouseWheelEvent& mouseWheelEvent)
    {
        mouseWheelEvents.push_back(mouseWheelEvent);
    }
    void Window::AddMouseButtonEvent(const MouseButtonEvent& mouseButtonEvent)
    {
        mouseButtonEvents.push_back(mouseButtonEvent);
    }
    void Window::AddKeyCodeEvent(const KeyCodeEvent& keyCodeEvent)
    {
        keyCodeEvents.push_back(keyCodeEvent);
    }
    void Window::AddCharacterEvent(const CharacterEvent& characterEvent)
    {
        characterEvents.push_back(characterEvent);
    }
    void Window::AddNativeEvent(const NativeEvent& characterEvent)
    {
        nativeEvents.push_back(characterEvent);
    }

    void Window::Show() const
    {
        //显示窗口
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        windows.emplace(const_cast<Window*>(this));
    }

    void Window::UpdateCursor() const
    {
        if (cursorVisible)
            SetCursor(hCursor);
        else
            SetCursor(nullptr);
    }

    LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
    {
        for (auto& nativeEvent : nativeEvents)
            nativeEvent(hwnd, messageSign, wparameter, lparameter);

        switch (messageSign)
        {
        case WM_PAINT:
            {
                for (auto& renewEvent : renewEvents)
                    renewEvent();

                //为了让用户能获取到鼠标增量，放后执行
                if (cursorlock)
                    SetCursorLocalPosition({lockCursorPos.x, lockCursorPos.y});
                else
                    lastCursorPos = cursorPos;
                break;
            }
        case WM_MOUSEMOVE:
            {
                cursorPos.x = static_cast<float>(lparameter & 0xffff);
                cursorPos.y = static_cast<float>(lparameter >> 16);
                for (auto& mouseMoveEvent : mouseMoveEvents)
                    mouseMoveEvent(cursorPos);
                break;
            }
        case WM_CHAR:
            {
                const auto character = static_cast<wchar_t>(wparameter);
                for (auto& characterEvent : characterEvents)
                    characterEvent(character);
                break;
            }
        case WM_KEYDOWN:
            {
                const auto keyCode = static_cast<KeyCode>(wparameter);
                for (auto& keyCodeEvent : keyCodeEvents)
                    keyCodeEvent(keyCode, true);
                break;
            }
        case WM_KEYUP:
            {
                const auto keyCode = static_cast<KeyCode>(wparameter);
                for (auto& keyCodeEvent : keyCodeEvents)
                    keyCodeEvent(keyCode, false);
                break;
            }
#pragma region 鼠标事件（除了移动鼠标事件）
        case WM_LBUTTONDOWN:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(0, true);
                break;
            }
        case WM_LBUTTONUP:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(0, false);
                break;
            }
        case WM_RBUTTONDOWN:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(1, true);
                break;
            }
        case WM_RBUTTONUP:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(1, false);
                break;
            }

        case WM_MOUSEWHEEL:
            {
                const Vector2 delta = {0,GET_WHEEL_DELTA_WPARAM(wparameter) / 120.0f};
                for (auto& mouseWheelEvent : mouseWheelEvents)
                    mouseWheelEvent(delta);
                break;
            }
        case WM_MBUTTONDOWN:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(2, true);
                break;
            }
        case WM_MBUTTONUP:
            {
                for (auto& mouseButtonEvent : mouseButtonEvents)
                    mouseButtonEvent(2, false);
                break;
            }
#pragma endregion
        case WM_SETCURSOR:
            {
                if (LOWORD(lparameter) == HTCLIENT)
                    UpdateCursor();
                break;
            }
        case WM_SIZE:
            {
                InvalidateRect(GetHwnd(), nullptr, true);

                const Vector2 size = {
                    static_cast<float>(lparameter & 0xffff),
                    static_cast<float>(lparameter >> 16)
                };
                for (auto& resizeEvent : resizeEvents)
                    resizeEvent(size);
                break;
            }
        case WM_DESTROY:
            {
                for (auto& destroyEvent : destroyEvents)
                    destroyEvent();

                windows.erase(this);
                Debug::LogWarning(L"Des");
                break;
            }
        default:
            break;
        }

        return DefWindowProcW(GetHwnd(), messageSign, wparameter, lparameter);
    }
    LRESULT Window::WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
    {
        Window* window;

        //获取窗口实例
        if (messageSign == WM_NCCREATE)
        {
            //第一次需转存实例
            const CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lparameter);
            window = static_cast<Window*>(createStruct->lpCreateParams);
            window->hwnd = hwnd;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
        else
        {
            window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }


        if (window != nullptr)
        {
            //使用窗口实例提供的消息处理程序
            return window->HandleMessage(messageSign, wparameter, lparameter);
        }
        else
        {
            return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
        }
    }
}
