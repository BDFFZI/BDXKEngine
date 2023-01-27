#include "Menu.h"

namespace BDXKEditor
{
    std::vector<std::tuple<std::string, std::function<void()>>>& Menu::GetMenuItems()
    {
        return menuItems;
    }
    void Menu::AddMenuItem(const std::string& name, const std::function<void()>& createFunc)
    {
        menuItems.emplace_back(name, createFunc);
    }
}
