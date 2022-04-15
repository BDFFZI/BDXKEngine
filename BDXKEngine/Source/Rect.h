#pragma once
#include "Vector2.h"

struct Rect
{
public:
	Vector2 position;
	Vector2 size;

	Vector2 GetMin() { return{ position.x,  position.y }; }
	Vector2 GetMax() { return{ position.x + size.x,  position.y + size.y }; }
	float GetXMin() { return position.x; };
	float GetYMin() { return position.y; };
	float GetXMax() { return position.x + size.x; };
	float GetYMax() { return position.y + size.y; };


	Rect()
	{
		position = {};
		size = {};
	}

	Rect(Vector2 min,Vector2 max)
	{
		position = min;
		size = max - min;
	}

	Rect(RECT rect)
	{
		position = { (float)rect.left,(float)rect.top };
		size = { (float)(rect.right - rect.left),(float)(rect.bottom - rect.top)};
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