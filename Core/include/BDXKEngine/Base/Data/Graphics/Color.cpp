#include "Color.h"

#include "BDXKEngine/Base/Data/Mathematics/Math.h"

namespace BDXKEngine
{
    const Color Color::clear = Color(0, 0, 0, 0);
    const Color Color::white = Color(1, 1, 1);
    const Color Color::black = Color(0, 0, 0);
    const Color Color::red = Color(1, 0, 0);
    const Color Color::green = Color(0, 1, 0);
    const Color Color::blue = Color(0, 0, 1);
    const Color Color::gray = Color(0.5f, 0.5f, 0.5f);
    const Color Color::yellow = Color(1, 1, 0);
    const Color Color::magenta = Color(1, 0, 1);
    const Color Color::lightRed = {1, 0.5f, 0.5f};
    const Color Color::lightGreen = {0.5f, 1, 0.5f};
    const Color Color::lightBlue = {0.5f, 0.5f, 1};
    const Color Color::lightYellow = {1, 1, 0.5f};
    const Color Color::darkRed = {0.5f, 0, 0};
    const Color Color::darkGreen = {0, 0.5f, 0};
    const Color Color::darkBlue = {0, 0, 0.5f};

    Color::Color() : Color(0, 0, 0, 0)
    {
    }
    Color::Color(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    Color::Color(float r, float g, float b) : Color(r, g, b, 1)
    {
    }

    std::string Color::ToString() const
    {
        std::stringstream string;
        string << '(' << r << ',' << g << ',' << b << ',' << a << ')';
        return string.str();
    }

    Color Color::operator*(float value) const
    {
        return {r * value, g * value, b * value, a * value};
    }
    Color Color::operator/(float value) const
    {
        return *this * (1 / value);
    }
    bool Color::operator==(Color value) const
    {
        return Equal(r, value.r) && Equal(g, value.g) && Equal(b, value.b) && Equal(a, value.a);
    }
    bool Color::operator!=(Color value) const
    {
        return !(*this == value);
    }
    Color::operator Vector4() const
    {
        return {r, g, b, a};
    }
}
