#pragma once
#include <d2d1.h>
#include "Vector2.h"

namespace BDXKEngine {
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



		operator Vector2()
		{
			return { (float)x,(float)y };
		}

		Vector2Int  operator/(float value)
		{
			return Vector2Int((int)(x / value), (int)(y / value));
		}
	};
}