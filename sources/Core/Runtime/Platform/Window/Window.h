#pragma once
#define BDXK_Platform_Windows//不支持跨平台

#include <functional>
#include <map>
#include <GLFW/glfw3.h>

#ifdef BDXK_Platform_Windows
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace BDXKEngine
{
    typedef std::function<void()> Action;

    class Window
    {
    public:
        static void Initialize(const char* name);
        static void Run(const std::function<bool()>& isOver = [] { return false; });

#ifdef BDXK_Platform_Windows
        static HWND GetHwnd();
#endif
        static void AddStartEvent(const Action& action, float order = 0);
        static void AddUpdateEvent(const Action& action, float order = 0);
        static void AddDestroyEvent(const Action& action, float order = 0);
        static GLFWwindow* GetGlfwWindow();

    private:
        inline static GLFWwindow* glfWwindow;
        inline static std::multimap<float, Action> startEvents;
        inline static std::multimap<float, Action> updateEvents;
        inline static std::multimap<float, Action> destroyEvents;
    };

    struct WindowEventRegister
    {
        WindowEventRegister(void (*addEvent)(const Action&, float), const std::function<void()>& event, const float order)
        {
            addEvent(event, order);
        }
    };


#define Internal_CustomWindowEvent(event,func,order,nameSuffix) inline static WindowEventRegister window##event##EventRegister##nameSuffix = {&Window::Add##event##Event,func,order};
#define Internal2_CustomWindowEvent(event,func,order,nameSuffix) Internal_CustomWindowEvent(event,func,order,nameSuffix)

#define CustomWindowStartEvent(func,order) Internal2_CustomWindowEvent(Start,func,order,__LINE__)
#define CustomWindowUpdateEvent(func,order) Internal2_CustomWindowEvent(Update,func,order,__LINE__)
#define CustomWindowDestoryEvent(func,order) Internal2_CustomWindowEvent(Destroy,func,order,__LINE__)
}
