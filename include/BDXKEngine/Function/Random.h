#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"

namespace BDXKEngine
{
    class Random
    {
    public:
        static Color ColorHSV();
        static Vector3 InsideUnitSphere();
        static Vector3 OnUnitSphere();

        static unsigned int seed;
    };
}
