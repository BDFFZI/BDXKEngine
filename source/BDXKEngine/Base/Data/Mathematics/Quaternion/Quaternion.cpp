#include "Quaternion.h"
#include "BDXKEngine/Base/Data/Mathematics/Math.h"

namespace BDXKEngine
{
    Quaternion Quaternion::Euler(Vector3 eulerAngles)
    {
        eulerAngles = eulerAngles / 2 * Deg2Rad();
        const Quaternion z = Quaternion(0, 0, sin(eulerAngles.z), cos(eulerAngles.z));
        const Quaternion x = Quaternion(sin(eulerAngles.x), 0, 0, cos(eulerAngles.x));
        const Quaternion y = Quaternion(0, sin(eulerAngles.y), 0, cos(eulerAngles.y));
        return y * x * z;
    }
    Quaternion::Quaternion(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Quaternion::Quaternion(float w, Vector3 xyz)
    {
        this->w = w;
        this->x = xyz.x;
        this->y = xyz.y;
        this->z = xyz.z;
    }
    float Quaternion::Angle() const
    {
        return acos(w) * Rad2Deg() * 2;
    }
    Vector3 Quaternion::Axis() const
    {
        const float factor = sin(acos(w));
        if (Equal(factor, 0))
            return Vector3::zero;
        return Vector3{x, y, z} / factor;
    }
    Quaternion Quaternion::operator*(Quaternion value) const
    {
        const Vector3 u = {x, y, z};
        const Vector3 v = {value.x, value.y, value.z};
        return Quaternion{w * value.w - Vector3::Dot(u, v), v * w + u * value.w + Vector3::Cross(u, v)};
    }
}
