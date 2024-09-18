#pragma once
#include <functional>
#include <string>
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Runtime/Base/Data/Graphics/Color.h"
#include "Core/Runtime/Base/Object/Pointer/ObjectPtrBase.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;
    class EditorMenu
    {
    public:
        static std::vector<std::tuple<std::string, std::function<void()>>>& GetMenuItems();
        static void AddMenuItem(const std::string& name, const std::function<void()>& createFunc);

        static void SetMenuBarColor(Color color);

    private:
        inline static std::vector<std::tuple<std::string, std::function<void()>>> menuItems = {};
        inline static Color barColor = {};

        static void DrawMenu(const std::vector<std::string>& path, const std::function<void()>& func, size_t layer = 0);

        static void OnEngineStart();
        static void OnEngineUpdate();
        CustomEngineStartEvent(OnEngineStart, 0)
        CustomEngineUpdateEvent(OnEngineUpdate, 0)
    };

    struct CustomMenuRegister
    {
        CustomMenuRegister(const std::string& name, const std::function<void()>& createFunc)
        {
            EditorMenu::AddMenuItem(name, createFunc);
        }
    };

    inline void NullMenuItem()
    {
    }

#define Internal_CustomMenuSeparator(menuItemName,line) inline static CustomMenuRegister CustomMenu##line = {menuItemName,NullMenuItem};
#define Internal_CustomMenuSeparator2(menuItemName,line) Internal_CustomMenuSeparator(menuItemName,line);

#define CustomMenuSeparator(menuItemName) Internal_CustomMenuSeparator2(menuItemName##"##",__LINE__)
#define CustomMenu(menuItemName,func) inline static CustomMenuRegister CustomMenu##func = {menuItemName,func};
}
