#pragma once
#include "Core/Runtime/Base/Data/Math/Vector/Vector3.h"

namespace BDXKEngine
{
    struct Quaternion
    {
        static Quaternion Euler(Vector3 eulerAngles);

        float x, y, z, w;

        Quaternion(float x, float y, float z, float w);
        Quaternion(float w, Vector3 xyz);

        float GetRotationAngle() const;
        Vector3 GetRotationAxis() const;
        
        Vector3 ToEulerAngles() const;

        Quaternion operator*(Quaternion value) const;
    };
}
