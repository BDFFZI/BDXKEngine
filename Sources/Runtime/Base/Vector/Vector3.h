#pragma once
#include<d2d1.h>
#include<iomanip>
#include"Vector2.h"

namespace BDXKEngine {
	struct Vector3
	{
	public:
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

		bool IsNaN();
		float GetMagnitude();
		Vector3 GetNormalized();

		Vector3 operator+(float value)
		{
			return Vector3(x + value, y + value, z + value);
		}
		Vector3 operator-(float value)
		{
			return Vector3(x - value, y - value, z - value);
		}
		Vector3 operator*(float value)
		{
			return Vector3(x * value, y * value, z * value);
		}
		Vector3 operator/(float value)
		{
			return Vector3(x / value, y / value, z / value);
		}


		Vector3 operator/=(float value) {
			return *this = *this / value;
		}


		Vector3 operator+(Vector3 value)
		{
			return Vector3(x + value.x, y + value.y, z + value.z);
		}
		Vector3 operator-(Vector3 value)
		{
			return Vector3(x - value.x, y - value.y, z - value.z);
		}
		Vector3 operator+=(Vector3 value) {
			return *this = *this + value;
		}

		Vector3 operator-()
		{
			return Vector3(-x, -y, -z);
		}

		bool operator==(Vector3 value)
		{
			return x == value.x && y == value.y && z == value.z;
		}
		bool operator!=(Vector3 value)
		{
			return x != value.x || y != value.y || z != value.z;
		}

		Vector3(D2D1_SIZE_F size);

		Vector3(POINT point);

		operator D2D1_POINT_2F();

		operator POINT();

		std::wstring ToString(int precision = 4)
		{
			std::wstringstream string;
			string << '('
				<< std::setprecision(precision) << x << ','
				<< std::setprecision(precision) << y << ','
				<< std::setprecision(precision) << z << ')';
			return string.str();
		}
	};
}

