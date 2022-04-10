#pragma once
#include<cmath>

class Math
{
public:
	static float Mod(float a, float b)
	{
		return std::fmod(a, b);
	}

	static float Sin(float radian)
	{
		return std::sin(radian);
	}

private:

};