#include "Window.h"
#include <cmath>

namespace BDXKEngine
{
    Window::Window(const std::wstring& name, const Window* parent)
    {
        this->nativeName = name.c_str();

        //向操作系统注册窗口类
        WNDCLASSW windowClass = {};
        windowClass.lpszClassName = nativeName; //类名，标识
        windowClass.lpfnWndProc = WindowProcess; //窗口消息处理函数
        RegisterClassW(&windowClass);

        //创建窗口
        hwnd = CreateWindowExW(
            0, //窗口行为
            nativeName, //窗口类名称
            nativeName, //窗口标题
            WS_OVERLAPPEDWINDOW, //窗口样式
            0, 0, //窗口位置xy
            1920, 1030, //窗口大小xy
            parent == nullptr ? nullptr : parent->GetHwnd(), //父窗口
            nullptr, //窗口菜单
            nullptr, //实例句柄,默认
            this //附加的数据
        );

        cursorPos = GetCursorLocalPosition();
        lastCursorPos = cursorPos;
    }

    std::wstring Window::GetName() const
    {
        return nativeName;
    }
    HWND Window::GetHwnd() const
    {
        return hwnd;
    }
    Rect Window::GetScreenRect() const
    {
        const Vector2 size = GetSize();

        POINT min{};
        POINT max = {static_cast<long>(size.x), static_cast<long>(size.y)};
        ClientToScreen(GetHwnd(), &min);
        ClientToScreen(GetHwnd(), &max);

        return Rect{
            static_cast<float>(min.x),
            static_cast<float>(min.y),
            static_cast<float>(max.x - min.x),
            static_cast<float>(max.y - min.y)
        };
    }
    Vector2 Window::GetSize() const
    {
        RECT rect;
        GetClientRect(GetHwnd(), &rect);
        return {static_cast<float>(rect.right), static_cast<float>(rect.bottom)};
    }
    Vector2 Window::GetCursorLocalPosition() const
    {
        const Vector2 origin = GetScreenRect().GetMin();

        POINT point;
        GetCursorPos(&point);

        return {static_cast<float>(point.x) - origin.x, static_cast<float>(point.y) - origin.y};
    }
    Vector2 Window::GetCursorMoveDelta() const
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
            const Rect rect = GetScreenRect();
            const RECT rectTemp = {
                static_cast<LONG>(rect.GetXMin()),
                static_cast<LONG>(rect.GetYMin()),
                static_cast<LONG>(rect.GetXMax()),
                static_cast<LONG>(rect.GetYMax())
            };
            ClipCursor(&rectTemp);
        }
        else
        {
            ClipCursor(nullptr);
        }
    }
    void Window::SetCursorLock(bool state)
    {
        //SetCursorLocalPosition(GetSize() / 2);
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
        //处理窗口事件
        MSG msg = {};
        while (GetMessage(&msg, hwnd, 0, 0) > 0)
        {
            //预处理后交给窗口过程响应
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            //如果没有要处理的消息，我们就用这段空闲时间更新游戏
            if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
                PostMessage(hwnd, WM_PAINT, NULL, NULL);
        }
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
                const auto character = static_cast<char>(wparameter);
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
                const Vector2 delta = {0.0f,GET_WHEEL_DELTA_WPARAM(wparameter) / 120.0f};
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
