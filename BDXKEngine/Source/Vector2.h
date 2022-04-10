#pragma once
#include <d2d1.h>
struct Vector2
{
public:
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

	Vector2(D2D1_SIZE_F size) :Vector2(size.width, size.height)
	{

	}

	operator D2D1_POINT_2F()
	{
		return D2D1::Point2F(x, y);
	}

	Vector2  operator/(float value)
	{
		return Vector2(x / value, y / value);
	}
};