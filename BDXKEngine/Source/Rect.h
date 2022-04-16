#pragma once
#include "Vector2.h"

struct Rect
{
public:
	float x;
	float y;
	float width;
	float height;

	Vector2 GetPosition() { return{ x,  y }; }
	Vector2 GetSize() { return{ width,  height }; }
	Vector2 GetMin() { return{ x,  y }; }
	Vector2 GetMax() { return{ x + width,  y + height }; }
	float GetXMin() { return x; };
	float GetYMin() { return y; };
	float GetXMax() { return x + width; };
	float GetYMax() { return y + height; };


	Rect()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rect(Vector2 min, Vector2 max)
	{
		x = min.x;
		y = min.y;
		width = max.x - min.x;
		height = max.y - min.y;
	}

	Rect(RECT rect)
	{
		x = (float)rect.left;
		y = (float)rect.top;
		width = (float)(rect.right - rect.left);
		height = (float)(rect.bottom - rect.top);
	}

	operator RECT()
	{
		RECT rect{};
		rect.left = (LONG)GetXMin();
		rect.right = (LONG)GetXMax();
		rect.top = (LONG)GetYMin();
		rect.bottom = (LONG)GetYMax();

		return rect;
	}
};