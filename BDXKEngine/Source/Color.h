#pragma once
#include <d2d1.h>
#include<sstream>

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

	Color(float r, float g, float b, float a);
	Color(float r, float g, float b);

	const wchar_t* ToString();

	operator D2D1_COLOR_F();
};