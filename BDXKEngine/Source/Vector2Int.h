#pragma once
#include <d2d1.h>
#include "Vector2.h"

struct Vector2Int
{
public:
	int x;
	int y;

	Vector2Int()
	{
		x = 0;
		y = 0;
	}

	Vector2Int(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2Int(D2D1_SIZE_U size) :Vector2Int(size.width, size.height)
	{

	}

	operator D2D1_SIZE_U()
	{
		return D2D1::SizeU(x, y);
	}

	Vector2Int  operator/(float value)
	{
		return Vector2Int((int)(x / value), (int)(y / value));
	}
};
