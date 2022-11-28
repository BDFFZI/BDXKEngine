#include "Vector3.h"
#include <iomanip>
#include <sstream>
#include "../Math.h"

namespace BDXKEngine
{
    const Vector3 Vector3::nan = {
        std::nanf(nullptr),
        std::nanf(nullptr),
        std::nanf(nullptr)
    };

    const Vector3 Vector3::zero = {0, 0, 0};
    const Vector3 Vector3::one = {1, 1, 1};
    const Vector3 Vector3::right = {1, 0, 0};
    const Vector3 Vector3::up = {0, 1, 0};
    const Vector3 Vector3::front = {0, 0, 1};
    const Vector3 Vector3::left = {-1, 0, 0};
    const Vector3 Vector3::down = {0, -1, 0};
    const Vector3 Vector3::back = {0, 0, -1};


    Vector3::Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3::Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3::Vector3(Vector2 value)
    {
        x = value.x;
        y = value.y;
        z = 0;
    }


    float Vector3::GetSqrMagnitude() const
    {
        return x * x + y * y + z * z;
    }
    float Vector3::GetMagnitude() const
    {
        return std::sqrtf(x * x + y * y + z * z);
    }
    Vector3 Vector3::GetNormalized() const
    {
        return *this / GetMagnitude();
    }
    bool Vector3::IsNaN() const
    {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    std::string Vector3::ToString() const
    {
        std::stringstream string;
        string << '('
            << std::setprecision(4) << x << ','
            << std::setprecision(4) << y << ','
            << std::setprecision(4) << z << ')';
        return string.str();
    }

    Vector3 Vector3::operator+(float value) const
    {
        return {x + value, y + value, z + value};
    }
    Vector3 Vector3::operator-(float value) const
    {
        return {x - value, y - value, z - value};
    }
    Vector3 Vector3::operator*(float value) const
    {
        return {x * value, y * value, z * value};
    }
    Vector3 Vector3::operator/(float value) const
    {
        return {x / value, y / value, z / value};
    }
    Vector3 Vector3::operator+(Vector3 value) const
    {
        return {x + value.x, y + value.y, z + value.z};
    }
    Vector3 Vector3::operator-(Vector3 value) const
    {
        return {x - value.x, y - value.y, z - value.z};
    }
    Vector3 Vector3::operator-() const
    {
        return {-x, -y, -z};
    }
    bool Vector3::operator==(Vector3 value) const
    {
        return Equal(x, value.x) && Equal(y, value.y) && Equal(z, value.z);
    }
    bool Vector3::operator!=(Vector3 value) const
    {
        return !(*this == value);
    }
    Vector3 Vector3::operator+=(Vector3 value)
    {
        return *this = *this + value;
    }
    Vector3 Vector3::operator*=(float value)
    {
        return *this = *this * value;
    }
    Vector3 Vector3::operator/=(float value)
    {
        return *this = *this / value;
    }
}
