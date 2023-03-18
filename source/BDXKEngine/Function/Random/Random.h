#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"

namespace BDXKEngine
{
    class Random
    {
    public:
        static float FloatValue(float min = 0, float max = 1);
        static int IntValue(int min = 0, int max = 1);
        static Color ColorHSV();
        static Vector3 SphereInside();
        static Vector3 SphereSurface(); //半径为一的球

        static unsigned int seed;
    };
}
