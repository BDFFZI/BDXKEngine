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
	static const Color gray;
	static const Color yellow;
	static const Color lightRed;
	static const Color lightGreen;
	static const Color lightBlue;

	float r;
	float g;
	float b;
	float a;

	Color(float r, float g, float b, float a);
	Color(float r, float g, float b);
	Color();

	const wchar_t* ToString();

	operator D2D1_COLOR_F();
};