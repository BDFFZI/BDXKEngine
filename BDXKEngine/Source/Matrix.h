#pragma once
#include<d2d1.h>
#include "Vector2.h"

struct Matrix
{
public:
	static const Matrix identity;

	static Matrix Rotate(float angle, Vector2 center)
	{
		return D2D1::Matrix3x2F::Rotation(angle, center);
	}

	static Matrix Translate(Vector2 move)
	{
		return D2D1::Matrix3x2F::Translation(move.x, move.y);
	}

	Matrix()
	{
		matrix = {};
	}

	Matrix(D2D1_MATRIX_3X2_F matrix)
	{
		this->matrix = matrix;
	}

	operator D2D1_MATRIX_3X2_F()
	{
		return matrix;
	}

	Matrix operator*(Matrix appendMatrix)
	{
		return matrix * appendMatrix;
	}

	Matrix operator*=(Matrix appendMatrix)
	{
		return matrix = matrix * appendMatrix;
	}

private:
	D2D1_MATRIX_3X2_F matrix;
};