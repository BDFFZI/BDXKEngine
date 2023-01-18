#include "ConsoleWindow.h"

#include <iostream>

#include "BDXKEngine/Base/Data/Mathematics/Math.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    std::stringstream& ConsoleWindow::GetConsole()
    {
        return stringstream;
    }

    void ConsoleWindow::OnGUI()
    {
        static char buffer[128];
        static std::vector<std::string> logs = {};
        static float lastScrollMaxY = 0;

        if (ImGui::Button("Clear"))
            logs.clear();

        ImGui::BeginChild("##");

        if (Equal(ImGui::GetScrollY(), lastScrollMaxY))
            ImGui::SetScrollY(ImGui::GetScrollMaxY());
        lastScrollMaxY = ImGui::GetScrollMaxY();

        if (stringstream.str().empty() == false)
            stringstream.clear();
        while (stringstream.eof() == false)
        {
            stringstream.getline(buffer, sizeof(buffer));
            if (buffer[0] != '\0')logs.emplace_back(buffer);
        }
        stringstream.str("");

        for (auto& item : logs)
            ImGui::Text(item.c_str());
        if (logs.size() > 1000)
            logs.erase(logs.begin(), logs.begin() + static_cast<std::ptrdiff_t>(logs.size() - 1000));

        ImGui::EndChild();
    }

    void ConsoleWindow::OnAwake()
    {
    }
    void ConsoleWindow::StaticConstructor()
    {
        std::cout.rdbuf(GetConsole().rdbuf());
    }
}
