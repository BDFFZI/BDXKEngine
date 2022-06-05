#pragma once
#include<d2d1.h>
#include"Vector2.h"

struct Vector3
{
public:
	static const Vector3 nan;//不要拿这个用作比较对象，nan无法比较，请使用IsNaN()
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;
	static const Vector3 left;
	static const Vector3 down;
	static const Vector3 back;

	float x;
	float y;
	float z;

	Vector3();

	Vector3(float x, float y, float z);
	Vector3(Vector2 value);

	bool IsNaN();

	Vector3 operator+(float value)
	{
		return Vector3(x + value, y + value, z + value);
	}
	Vector3 operator-(float value)
	{
		return Vector3(x - value, y - value, z - value);
	}
	Vector3 operator*(float value)
	{
		return Vector3(x * value, y * value, z * value);
	}
	Vector3 operator/(float value)
	{
		return Vector3(x / value, y / value, z / value);
	}

	Vector3 operator+(Vector3 value)
	{
		return Vector3(x + value.x, y + value.y, z + value.z);
	}
	Vector3 operator-(Vector3 value)
	{
		return Vector3(x - value.x, y - value.y, z - value.z);
	}

	bool operator==(Vector3 value)
	{
		return x == value.x && y == value.y && z == value.z;
	}
	bool operator!=(Vector3 value)
	{
		return x != value.x || y != value.y || z != value.z;
	}

	const wchar_t* ToString();

	Vector3(D2D1_SIZE_F size);

	operator D2D1_POINT_2F();

	Vector3(POINT point);
	operator POINT();
};

