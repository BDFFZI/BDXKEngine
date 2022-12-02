#pragma once
#include <cmath>
#include <corecrt_math_defines.h>

namespace BDXKEngine
{
    inline int precision = 3;
    inline bool Equal(float a, float b)
    {
        return abs(a - b) < 0.001f;
    }
    inline float Deg2Rad()
    {
        return static_cast<float>(M_PI) / 180;
    }
    inline float Rad2Deg()
    {
        return 180 / static_cast<float>(M_PI);
    }
}
