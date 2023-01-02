#pragma once
#include <functional>
#include <string>

namespace BDXKEditor
{
    template <typename TFunc>
    class Menu
    {
    public:
        static void AddMenuItem(const std::string& name, const std::function<TFunc>& createFunc)
        {
            menuItems.emplace_back(name, createFunc);
        }
        static std::vector<std::tuple<std::string, std::function<TFunc>>>& GetMenuItems()
        {
            return menuItems;
        }
    private:
        inline static std::vector<std::tuple<std::string, std::function<TFunc>>> menuItems = {};
    };

    template <typename TFunc>
    struct CustomMenuRegister
    {
        CustomMenuRegister(const std::string& name, const std::function<TFunc>& createFunc)
        {
            Menu<TFunc>::AddMenuItem(name, createFunc);
        }
    };

#define CustomMenu(funcType,funcName) inline CustomMenuRegister<##funcType> CustomMenu##funcName = {#funcName,funcName};
}
