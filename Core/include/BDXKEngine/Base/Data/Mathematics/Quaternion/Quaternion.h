#pragma once
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"

namespace BDXKEngine
{
    struct Quaternion
    {
        static Quaternion Euler(Vector3 eulerAngles);

        float x, y, z, w;

        Quaternion(float x, float y, float z, float w);
        Quaternion(float w, Vector3 xyz);

        float Angle() const;
        Vector3 Axis() const;

        Quaternion operator*(Quaternion value) const;
    };
}
