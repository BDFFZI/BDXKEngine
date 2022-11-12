#include "Vector2.h"
#include <sstream>
#include "../Math.h"

namespace BDXKEngine
{
    const Vector2 Vector2::zero = Vector2(0, 0);
    const Vector2 Vector2::one = Vector2(1, 1);
    const Vector2 Vector2::right = Vector2(1, 0);
    const Vector2 Vector2::up = Vector2(0, 1);
    const Vector2 Vector2::left = Vector2(-1, 0);
    const Vector2 Vector2::down = Vector2(0, -1);

    Vector2::Vector2()
    {
        x = 0;
        y = 0;
    }
    Vector2::Vector2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    Vector2::Vector2(int x, int y)
    {
        this->x = static_cast<float>(x);
        this->y = static_cast<float>(y);
    }

    int Vector2::GetXInt() const
    {
        return static_cast<int>(std::round(x));
    }
    int Vector2::GetYInt() const
    {
        return static_cast<int>(std::round(y));
    }

    std::string Vector2::ToString() const
    {
        std::stringstream string;
        string << '(' << x << ',' << y << ')';
        return string.str();
    }

    Vector2 Vector2::operator+(float value) const
    {
        return {x + value, y + value};
    }
    Vector2 Vector2::operator-(float value) const
    {
        return {x - value, y - value};
    }
    Vector2 Vector2::operator*(float value) const
    {
        return {x * value, y * value};
    }
    Vector2 Vector2::operator/(float value) const
    {
        return {x / value, y / value};
    }
    Vector2 Vector2::operator+(Vector2 value) const
    {
        return {x + value.x, y + value.y};
    }
    Vector2 Vector2::operator-(Vector2 value) const
    {
        return {x - value.x, y - value.y};
    }
    bool Vector2::operator==(Vector2 value) const
    {
        return Equal(x, value.x) && Equal(y, value.y);
    }
}
