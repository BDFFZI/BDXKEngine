#pragma once
#include <d2d1.h>
struct Color
{
public:
	static const Color clear;
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;

	float r;
	float g;
	float b;
	float a;

	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color(float r, float g, float b) :Color(r, g, b, 1)
	{

	}

	operator D2D1_COLOR_F()
	{
		return D2D1::ColorF(r, g, b, a);
	}


};