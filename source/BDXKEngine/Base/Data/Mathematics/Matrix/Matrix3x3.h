#pragma once

namespace BDXKEngine
{
    struct Matrix3x3
    {
        Matrix3x3();
        Matrix3x3(
            float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);

        float m00, m10, m20;
        float m01, m11, m21;
        float m02, m12, m22;

        float GetDeterminant() const;
    };
}
