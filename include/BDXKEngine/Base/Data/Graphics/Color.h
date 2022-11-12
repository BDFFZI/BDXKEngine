#pragma once
#include<sstream>

namespace BDXKEngine
{
    struct Color
    {
        static const Color clear;
        static const Color white;
        static const Color black;
        static const Color red;
        static const Color green;
        static const Color blue;
        static const Color gray;
        static const Color yellow;
        static const Color lightRed;
        static const Color lightGreen;
        static const Color lightBlue;
        static const Color lightYellow;

        float r;
        float g;
        float b;
        float a;

        Color(float r, float g, float b, float a);
        Color(float r, float g, float b);
        Color();

        std::string ToString() const;

        Color operator*(float value) const;
        Color operator/(float value) const;
    };
}
