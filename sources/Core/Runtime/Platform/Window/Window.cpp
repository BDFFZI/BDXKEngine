#include "Window.h"
#include <ranges>
#include "Cursor.h"
#include "Input.h"
#include "Screen.h"
#include "Time.h"
#include "Core/Runtime/Base/Debug/Debug.h"

namespace BDXKEngine
{
    void Window::Initialize(const char* name)
    {
        glfwSetErrorCallback(Debug::LogError);
        if (glfwInit() == false)
            throw std::exception("初始化失败");

        glfWwindow = glfwCreateWindow(1920 - 200, 1030 - 200, name, nullptr, nullptr);
        if (glfWwindow == nullptr)
            throw std::exception("创建窗口失败");

        Cursor::Initialize(glfWwindow);
        Screen::Initialize(glfWwindow);
        Input::Initialize(glfWwindow);
    }
    void Window::Run(const std::function<bool()>& isOver)
    {
        for (auto& item : startEvents | std::views::values)
            item();

        while (glfwWindowShouldClose(glfWwindow) == false)
        {
            glfwPollEvents();
            Input::Update();
            Cursor::Update();
            Time::Update();

            for (auto& item : updateEvents | std::views::values)
                item();

            if (isOver())
                break;
        }

        for (auto& item : destroyEvents | std::views::values)
            item();

        glfwDestroyWindow(glfWwindow);
        glfwTerminate();
    }

#ifdef BDXK_Platform_Windows
    HWND Window::GetHwnd()
    {
        return glfwGetWin32Window(glfWwindow);
    }
#endif

    void Window::AddStartEvent(const Action& action, float order)
    {
        startEvents.insert({order, action});
    }
    void Window::AddUpdateEvent(const Action& action, float order)
    {
        updateEvents.insert({order, action});
    }
    void Window::AddDestroyEvent(const Action& action, float order)
    {
        destroyEvents.insert({order, action});
    }
    GLFWwindow* Window::GetGlfwWindow()
    {
        return glfWwindow;
    }
}
