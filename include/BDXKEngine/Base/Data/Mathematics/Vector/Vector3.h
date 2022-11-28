#pragma once
#include "Vector2.h"

namespace BDXKEngine
{
    struct Vector3
    {
        /// <summary>
        /// 不要拿这个用作比较对象，nan无法比较，请使用IsNaN()
        /// </summary>
        static const Vector3 nan;
        static const Vector3 zero;
        static const Vector3 one;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 left;
        static const Vector3 right;
        static const Vector3 front;
        static const Vector3 back;

        float x;
        float y;
        float z;

        Vector3();
        Vector3(float x, float y, float z);
        Vector3(Vector2 value);

        float GetSqrMagnitude()const;
        float GetMagnitude() const;
        Vector3 GetNormalized() const;
        bool IsNaN() const;

        std::string ToString() const;

        Vector3 operator+(float value) const;
        Vector3 operator-(float value) const;
        Vector3 operator*(float value) const;
        Vector3 operator/(float value) const;
        Vector3 operator+(Vector3 value) const;
        Vector3 operator-(Vector3 value) const;
        Vector3 operator-() const;
        bool operator==(Vector3 value) const;
        bool operator!=(Vector3 value) const;
        Vector3 operator+=(Vector3 value);
        Vector3 operator*=(float value);
        Vector3 operator/=(float value);
    };
}
