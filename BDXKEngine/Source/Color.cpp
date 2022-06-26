#include "Color.h"

const Color Color::clear = Color(0, 0, 0, 0);
const Color Color::white = Color(1, 1, 1);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(1, 0, 0);
const Color Color::green = Color(0, 1, 0);
const Color Color::blue = Color(0, 0, 1);

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
Color::Color(float r, float g, float b) :Color(r, g, b, 1)
{

}

Color::Color() : Color(0, 0, 0, 0)
{
}

Color::operator D2D1_COLOR_F()
{
	return D2D1::ColorF(r, g, b, a);
}

const wchar_t* Color::ToString()
{
	std::wstringstream string;
	string << '(' << r << ',' << g << ',' << b << ',' << a << ')';
	return string.str().c_str();
}