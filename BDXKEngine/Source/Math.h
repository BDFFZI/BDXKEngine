#pragma once

class Math
{
public:
	static const float Pi;
	static const float Deg2Rad;
	static const float Rad2Deg;

	static float Mod(float a, float b);
	static float Max(float a, float b);
	static float Min(float a, float b);
	static float Clamp(float value, float min, float max);
	static float Clamp01(float value);

	static float Sin(float radian);
	static float Cos(float radian);
private:

};