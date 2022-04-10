#pragma once
#include <d2d1.h>
struct Color
{
public:
	float R;
	float G;
	float B;
	float A;

	Color(float r, float g, float b, float a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	Color(float r, float g, float b) :Color(r, g, b, 1)
	{

	}

	operator D2D1_COLOR_F()
	{
		return D2D1::ColorF(R, G, B, A);
	}

	static const Color Clear;
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};