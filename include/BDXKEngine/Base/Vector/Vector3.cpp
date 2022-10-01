#include "Vector3.h"
#include <cmath>

namespace BDXKEngine {
	const Vector3 Vector3::nan = {
		std::nanf(NULL),
		std::nanf(NULL),
		std::nanf(NULL)
	};

	const Vector3 Vector3::zero = { 0,0,0 };
	const Vector3 Vector3::one = { 1,1,1 };
	const Vector3 Vector3::right = { 1,0,0 };
	const Vector3 Vector3::up = { 0,1,0 };
	const Vector3 Vector3::front = { 0,0,1 };
	const Vector3 Vector3::left = { -1,0,0 };
	const Vector3 Vector3::down = { 0,-1,0 };
	const Vector3 Vector3::back = { 0,0,-1 };


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

	bool Vector3::IsNaN()
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	float Vector3::GetMagnitude()
	{
		return std::sqrtf(x * x + y * y + z * z);
	}

	Vector3 Vector3::GetNormalized()
	{
		return *this / GetMagnitude();
	}

	Vector3::Vector3(D2D1_SIZE_F size) :Vector3(size.width, size.height, 0) {
	}

	Vector3::operator D2D1_POINT_2F()
	{
		return D2D1::Point2F(x, y);
	}

	Vector3::Vector3(POINT point) : Vector3((float)point.x, (float)point.y, 0) {
	}
	Vector3::operator POINT()
	{
		POINT point{};
		point.x = (LONG)x;
		point.y = (LONG)y;

		return point;
	}
}