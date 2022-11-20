#pragma once
#include "Vector3.h"

namespace BDXKEngine
{
    struct Vector4
    {
        float x;
        float y;
        float z;
        float w;

        Vector4();
        Vector4(float x, float y, float z, float w);
        Vector4(Vector3 vector3, float w);
        Vector4(Vector2 vector, Vector2 vector2 = {});

        explicit operator Vector3();
    };
}
