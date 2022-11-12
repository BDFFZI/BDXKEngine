#pragma once
#include <cmath>
#include <corecrt_math_defines.h>

namespace BDXKEngine
{
    inline int precision = 4;
    inline bool Equal(float a, float b)
    {
        return abs(a - b) < 0.0001f;
    }
    inline float Deg2Rad(float degree)
    {
        return degree / 180 * static_cast<float>(M_PI);
    }
}
