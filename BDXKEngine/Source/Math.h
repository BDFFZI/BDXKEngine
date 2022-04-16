#pragma once
#include<cmath>

class Math
{
public:
	static const float Pi;
	static const float Deg2Rad;
	static const float Rad2Deg;

	static float Mod(float a, float b)
	{
		return std::fmod(a, b);
	}

	static float Sin(float radian)
	{
		return std::sin(radian);
	}

	static float Cos(float radian)
	{
		return std::cos(radian);
	}

private:

};