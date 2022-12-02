#include "ConsoleWindow.h"

#include <iostream>

#include "BDXKEngine/Base/Data/Mathematics/Math.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    std::stringstream stringstream;
    char buffer[128];
    std::vector<std::string> logs;
    float lastScrollMaxY;

    std::stringstream& ConsoleWindow::GetConsole()
    {
        return stringstream;
    }

    void ConsoleWindow::OnGUI()
    {
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
    }

    void ConsoleWindow::OnAwake()
    {
    }
}
