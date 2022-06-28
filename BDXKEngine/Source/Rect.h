#pragma once
#include "Vector2.h"

struct Rect
{
	Rect();
	Rect(Vector2 min, Vector2 max);
	Rect(RECT rect);

	float x;
	float y;
	float width;
	float height;

	Vector2 GetPosition();
	Vector2 GetSize();
	Vector2 GetMin();
	Vector2 GetMax();
	float GetXMin();
	float GetYMin();
	float GetXMax();
	float GetYMax();

	void SetSize(Vector2 size);
	void SetPosition(Vector2 position);

	operator RECT();
};