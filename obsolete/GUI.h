#pragma once
#include "BDXKEngine/Base/Rect.h"
#include "BDXKEngine/Platform/GL/GL2D.h"
#include "BDXKEngine/Platform/IMGUI/IMGUIManager.h"
#include "Event.h"

namespace BDXKEngine
{
    class GUI
    {
    public:
        static bool Button(Rect rect, std::string text);
        static std::string TextArea(Rect rect, std::string text, int fontSize = 20);
        static float Slider(Rect rect, float value, float leftValue, float rightValue);
        template <typename TValue>
        static void Array(Rect rect, TValue* array, int count)
        {
            std::stringstream stream{};
            stream << '{';
            for (int i = 0; i < count; i++)
                stream << std::to_string(*(array + i)) << ',';
            stream << '}';

            TextArea(rect, stream.str());
        }
    protected:
        static void Initialize(Window* window);
    private:
        static Window* window;
        static std::stringstream charStream;
        static int deleteStream;
    };
}
