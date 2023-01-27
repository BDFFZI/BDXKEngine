#pragma once
#include <functional>
#include <string>

namespace BDXKEditor
{
    class Menu
    {
    public:
        static std::vector<std::tuple<std::string, std::function<void()>>>& GetMenuItems();
        static void AddMenuItem(const std::string& name, const std::function<void()>& createFunc);
    private:
        inline static std::vector<std::tuple<std::string, std::function<void()>>> menuItems = {};
    };

    struct CustomMenuRegister
    {
        CustomMenuRegister(const std::string& name, const std::function<void()>& createFunc)
        {
            Menu::AddMenuItem(name, createFunc);
        }
    };

    inline void NullMenuItem()
    {
    }

#define CustomMenu(menuItemName,func) inline static CustomMenuRegister CustomMenu##func = {menuItemName,func};
#define Internal_CustomMenuSeparator(menuItemName,line) inline static CustomMenuRegister CustomMenu##line = {menuItemName,NullMenuItem};
#define Internal_CustomMenuSeparator2(menuItemName,line) Internal_CustomMenuSeparator(menuItemName,line);
#define CustomMenuSeparator(menuItemName) Internal_CustomMenuSeparator2(menuItemName##"##",__LINE__)
}
