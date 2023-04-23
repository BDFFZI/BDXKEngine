#include "Vector4.h"

namespace BDXKEngine
{
    Vector4::Vector4()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    Vector4::Vector4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vector4::Vector4(Vector3 vector3, float w)
    {
        x = vector3.x;
        y = vector3.y;
        z = vector3.z;
        this->w = w;
    }
    Vector4::Vector4(Vector2 vector, Vector2 vector2)
    {
        x = vector.x;
        y = vector.y;
        z = vector2.x;
        w = vector2.y;
    }

    Vector4::operator Vector3()
    {
        return {x, y, z};
    }
}
