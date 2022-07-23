#pragma once
#include<d2d1.h>
#include<cmath>
#include "Vector2.h"

//https://docs.microsoft.com/en-us/windows/win32/learnwin32/appendix--matrix-transforms
//DirectX使用行向量
//[m11 m12] = [Xx Xy]X轴基向量
//[m21 m22] = [Yx Yy]Y轴基向量
//[m31 m32] = [Tx Ty]平移
// 
//乘法顺序：向量*矩阵
//相乘方式：左按列，右按行。故:左边矩阵的列数必须等于右边矩阵的列数
//

namespace BDXKEngine {
	struct Matrix3x2
	{
	public:
		static const Matrix3x2 identity;

		static Matrix3x2 Scale(Vector2 scale)
		{
			return {
				scale.x,0,
				0,scale.y,
				0,0
			};
		}
		static Matrix3x2 Rotate(float degree)
		{
			float radian = degree / 180 * M_PI;
			return {
				std::cos(radian),std::sin(radian),
				-std::sin(radian),std::cos(radian),
				0,0
			};
		}
		static Matrix3x2 Translate(Vector2 move)
		{
			return {
				1,0,
				0,1,
				move.x,move.y
			};
		}

		float m00;
		float m01;
		float m10;
		float m11;
		float m20;
		float m21;

		Matrix3x2()
		{
			m00 = 0;
			m01 = 0;
			m10 = 0;
			m11 = 0;
			m20 = 0;
			m21 = 0;
		}

		Matrix3x2(
			float m00, float m01,
			float m10, float m11,
			float m20, float m21
		)
		{
			this->m00 = m00;
			this->m01 = m01;
			this->m10 = m10;
			this->m11 = m11;
			this->m20 = m20;
			this->m21 = m21;
		}

		Matrix3x2(D2D1_MATRIX_3X2_F matrix)
		{
			m00 = matrix.m11;
			m01 = matrix.m12;
			m10 = matrix.m21;
			m11 = matrix.m22;
			m20 = matrix.dx;
			m21 = matrix.dy;
		}

		operator D2D1_MATRIX_3X2_F()
		{
			return D2D1::Matrix3x2F(
				m00, m01,
				m10, m11,
				m20, m21
			);
		}

		Matrix3x2 operator*(Matrix3x2 appendMatrix)
		{
			Matrix3x2 matrix;
			matrix.m00 = m00 * appendMatrix.m00 + m01 * appendMatrix.m10 + 0 * appendMatrix.m20;
			matrix.m01 = m00 * appendMatrix.m01 + m01 * appendMatrix.m11 + 0 * appendMatrix.m21;

			matrix.m10 = m10 * appendMatrix.m00 + m11 * appendMatrix.m10 + 0 * appendMatrix.m20;
			matrix.m11 = m10 * appendMatrix.m01 + m11 * appendMatrix.m11 + 0 * appendMatrix.m21;

			matrix.m20 = m20 * appendMatrix.m00 + m21 * appendMatrix.m10 + 1 * appendMatrix.m20;
			matrix.m21 = m20 * appendMatrix.m01 + m21 * appendMatrix.m11 + 1 * appendMatrix.m21;

			return matrix;
		}

		Matrix3x2 operator*=(Matrix3x2 appendMatrix)
		{
			return *this = *this * appendMatrix;
		}
	};
}