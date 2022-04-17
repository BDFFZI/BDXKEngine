#pragma once
#include <d2d1.h>
#include"Object.h"

class Vector2 :public Object
{
public:
	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 left;
	static const Vector2 right;
	static const Vector2 up;
	static const Vector2 down;

	float x;
	float y;

	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(D2D1_SIZE_F size) :Vector2(size.width, size.height) {}

	Vector2(POINT point) :Vector2((float)point.x, (float)point.y) {}

	operator D2D1_POINT_2F()
	{
		return D2D1::Point2F(x, y);
	}

	operator POINT()
	{
		POINT point{};
		point.x = (LONG)x;
		point.y = (LONG)y;

		return point;
	}

	Vector2 operator+(float value)
	{
		return Vector2(x + value, y + value);
	}
	Vector2 operator-(float value)
	{
		return Vector2(x - value, y - value);
	}
	Vector2 operator*(float value)
	{
		return Vector2(x * value, y * value);
	}
	Vector2 operator/(float value)
	{
		return Vector2(x / value, y / value);
	}
	Vector2 operator+(Vector2 value)
	{
		return Vector2(x + value.x, y + value.y);
	}
	Vector2 operator-(Vector2 value)
	{
		return Vector2(x - value.x, y - value.y);
	}

	String ToString()override {
		std::wstringstream string;
		string << '(' << x << ',' << y << ')';
		return string.str();
	}
};