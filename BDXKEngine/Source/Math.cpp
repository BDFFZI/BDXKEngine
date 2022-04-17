#include "Math.h"
#include <cmath>
#include <algorithm>
#include "Exception.h"

const float Math::Pi = 3.1415926535897932384626433832795f;
const float Math::Deg2Rad = Pi / 180;
const float Math::Rad2Deg = 180 / Pi;


float Math::Mod(float a, float b)
{
	return std::fmod(a, b);
}
float Math::Max(float a, float b) {
	return std::fmax(a, b);
}
float Math::Min(float a, float b) {
	return std::fmin(a, b);
}
float Math::Clamp(float value, float min, float max)
{
	if (min > max)
		throw MisuseException(L"min不能大于max");

	if (value < min)
		value = min;
	else if (value > max)
		value = max;

	return value;
}
float Math::Clamp01(float value)
{
	return Clamp(value, 0, 1);
}

float Math::Sin(float radian)
{
	return std::sin(radian);
}
float Math::Cos(float radian)
{
	return std::cos(radian);
}


