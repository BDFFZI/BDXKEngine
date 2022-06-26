#include "Rect.h"

Rect::Rect()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}
Rect::Rect(Vector2 min, Vector2 max)
{
	x = min.x;
	y = min.y;
	width = max.x - min.x;
	height = max.y - min.y;
}
Rect::Rect(RECT rect)
{
	x = (float)rect.left;
	y = (float)rect.top;
	width = (float)(rect.right - rect.left);
	height = (float)(rect.bottom - rect.top);
}

Vector2 Rect::GetPosition() { return{ x,  y }; }
Vector2 Rect::GetSize() { return{ width,  height }; }
Vector2 Rect::GetMin() { return{ x,  y }; }
Vector2 Rect::GetMax() { return{ x + width,  y + height }; }
float Rect::GetXMin() { return x; }
float Rect::GetYMin() { return y; }
float Rect::GetXMax() { return x + width; }
float Rect::GetYMax() { return y + height; }

Rect::operator RECT()
{
	RECT rect{};
	rect.left = (LONG)GetXMin();
	rect.right = (LONG)GetXMax();
	rect.top = (LONG)GetYMin();
	rect.bottom = (LONG)GetYMax();

	return rect;
}
