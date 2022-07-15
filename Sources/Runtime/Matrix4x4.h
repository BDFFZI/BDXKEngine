#pragma once
#include <vector>
#include <string>
#include "Vector3.h"
#include "Math.h"
#include "Matrix3x2.h"
#include "Matrix3x3.h"

namespace BDXKEngine {
	struct Matrix4x4
	{
	public:
		static const Matrix4x4 zero;
		static const Matrix4x4 identity;

		static Matrix4x4 Scale(Vector3 scale);
		static Matrix4x4 Rotate(Vector3 degree);
		static Matrix4x4 Translate(Vector3 move);

		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;

		float GetElement(int row, int column);
		Matrix3x3 GetComplementMinor(int row, int column);
		float GetDeterminant();
		Matrix4x4 GetInverse();
		Matrix4x4 GetTranspose();
		Matrix3x2 ToMatrix3x2();
		Vector3 MultiplyVector(Vector3 value);


		Matrix4x4();

		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		Matrix4x4 operator*(Matrix4x4 append);
		Matrix4x4 operator*(float value);
		Matrix4x4 operator/(float value);
		Matrix4x4 operator*=(Matrix4x4 append);
		bool operator==(Matrix4x4 append);
		bool operator!=(Matrix4x4 append);

		std::wstring ToString();
	};
}

