#pragma once
#include <string>

namespace BDXKEngine
{
    struct Vector2
    {
        static const Vector2 zero;
        static const Vector2 one;
        static const Vector2 left;
        static const Vector2 right;
        static const Vector2 up;
        static const Vector2 down;

        float x;
        float y;

        Vector2();
        Vector2(float x, float y);
        Vector2(int x, int y);

        int GetXInt() const;
        int GetYInt() const;
        
        std::string ToString() const;

        Vector2 operator+(float value) const;
        Vector2 operator-(float value) const;
        Vector2 operator*(float value) const;
        Vector2 operator/(float value) const;
        Vector2 operator+(Vector2 value) const;
        Vector2 operator-(Vector2 value) const;
        Vector2 operator*(Vector2 value) const;
        bool operator==(Vector2 value) const;
    };
}
