#pragma once
#include "Vector2.h"

namespace BDXKEngine {
	struct Rect
	{
		static Rect zero;

		Rect();
		Rect(float x, float y, float width, float height);
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
		bool Contains(Vector2 position);

		void SetSize(Vector2 size);
		void SetPosition(Vector2 position);

		bool operator==(Rect rect);
		operator RECT();
	};
}