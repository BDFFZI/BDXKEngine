#pragma once
#include<sstream>
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"

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
        static const Color magenta;
        static const Color lightRed;
        static const Color lightGreen;
        static const Color lightBlue;
        static const Color lightYellow;
        static const Color darkRed;
        static const Color darkGreen;
        static const Color darkBlue;

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
        bool operator==(Color value) const;
        bool operator!=(Color value) const;
        
        operator Vector4() const;
    };
}
