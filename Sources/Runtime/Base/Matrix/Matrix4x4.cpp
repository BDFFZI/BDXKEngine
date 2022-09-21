#include <cmath>
#include "Matrix4x4.h"

namespace BDXKEngine {
	const Matrix4x4 Matrix4x4::zero = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
	};
	const Matrix4x4 Matrix4x4::identity = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
	};

	Matrix4x4 Matrix4x4::Scale(Vector3 scale)
	{
		return {
			scale.x,0,0,0,
			0,scale.y,0,0,
			0,0,scale.z,0,
			0,0,0,1
		};
	}
	Matrix4x4 Matrix4x4::Rotate(Vector3 degree)
	{
		Vector3 radian = degree / 180 * M_PI;


		float ry = radian.y;
		Matrix4x4 y = {
			std::cos(ry),0,std::sin(ry),0,
			0,1,0,0,
			-std::sin(ry),0,std::cos(ry),0,
			0,0,0,1
		};
		float rx = radian.x;
		Matrix4x4 x = {
			1,0,0,0,
			0,std::cos(rx),-std::sin(rx),0,
			0,std::sin(rx),std::cos(rx),0,
			0,0,0,1
		};
		float rz = radian.z;
		Matrix4x4 z = {
			std::cos(rz),-std::sin(rz),0,0,
			std::sin(rz),std::cos(rz),0,0,
			0,0,1,0,
			0,0,0,1
		};

		//旋转顺序很重要，用摄像机试一试就知道
		return y * x * z;
	}
	Matrix4x4 Matrix4x4::Translate(Vector3 move)
	{
		return {
			1,0,0,move.x,
			0,1,0,move.y,
			0,0,1,move.z,
			0,0,0,1
		};
	}

	Matrix4x4 Matrix4x4::Ortho(float halfWidth, float halfhHeight, float nearClipPlane, float farClipPlane)
	{
		float ClipPlaneParameterA = 1 / (farClipPlane - nearClipPlane);
		float ClipPlaneParameterB = -ClipPlaneParameterA * nearClipPlane;
		return {
			1 / halfWidth,0,0,0,
			0,1 / halfhHeight,0,0,
			0,0,ClipPlaneParameterA,ClipPlaneParameterB,
			0,0,0,1
		};
	}
	Matrix4x4 Matrix4x4::Perspective(float fieldOfView, float aspectRatio, float nearClipPlane, float farClipPlane)
	{
		float unitClipPlaneHalfHeight = std::tan(fieldOfView / 2 / 180 * M_PI);
		//裁剪面的作用是使当深度等于远截面时最终深度恰好为1，等于近截面时恰好为0
		//而最终深度计算结果=(az+b)/z
		//故我们的目标便是求该式中的a和b
		//列出二元一次方程组，利用代入消元法求解得出如下结论
		float ClipPlaneParameterB = farClipPlane * nearClipPlane / (nearClipPlane - farClipPlane);
		float ClipPlaneParameterA = -ClipPlaneParameterB / nearClipPlane;
		return {
			//控制视野范围并避免受窗口大小缩放影响
			1 / unitClipPlaneHalfHeight / aspectRatio ,0,0,0,
			0,1 / unitClipPlaneHalfHeight,0,0,
			0,0,ClipPlaneParameterA,ClipPlaneParameterB,
			//利用齐次坐标中的w分量实现近大远小公式 xy / z
			0,0,1,0
		};
	}
	Matrix4x4 Matrix4x4::TRS(Vector3 position, Vector3 eulerAngles, Vector3 scale)
	{
		//矩阵的计算顺序是从右到左[Translate(Rotate(Scale(vector)))]
		return Translate(position) * Rotate(eulerAngles) * Scale(scale);
	}

	float Matrix4x4::GetElement(int row, int column)
	{
		return ((float*)this)[row * 4 + column];
	}
	Matrix3x3 Matrix4x4::GetComplementMinor(int row, int column)
	{
		Matrix3x3 matrix{};

		float* matrixPointer = (float*)&matrix;
		int index = 0;
		for (int rowIndex = 0; rowIndex < 4; rowIndex++)
		{
			for (int columnIndex = 0; columnIndex < 4; columnIndex++)
			{
				if (rowIndex != row && columnIndex != column)
				{
					matrixPointer[index++] = GetElement(rowIndex, columnIndex);
				}
			}
		}

		return matrix;
	}
	float Matrix4x4::GetDeterminant()
	{
		//求余子式
		float det00 = GetComplementMinor(0, 0).GetDeterminant();
		float det01 = GetComplementMinor(0, 1).GetDeterminant();
		float det02 = GetComplementMinor(0, 2).GetDeterminant();
		float det03 = GetComplementMinor(0, 3).GetDeterminant();

		//转为代数余子式
		return m00 * det00 + m01 * -det01 + m02 * det02 + m03 * -det03;
	}
	Matrix4x4 Matrix4x4::GetInverse() {
		//求余子式
		float det00 = GetComplementMinor(0, 0).GetDeterminant();
		float det01 = GetComplementMinor(0, 1).GetDeterminant();
		float det02 = GetComplementMinor(0, 2).GetDeterminant();
		float det03 = GetComplementMinor(0, 3).GetDeterminant();

		//求行列式
		float det = m00 * det00 + m01 * -det01 + m02 * det02 + m03 * -det03;
		if (det == 0)
			return {};

		//求出全部余子式
		float det10 = GetComplementMinor(1, 0).GetDeterminant();
		float det11 = GetComplementMinor(1, 1).GetDeterminant();
		float det12 = GetComplementMinor(1, 2).GetDeterminant();
		float det13 = GetComplementMinor(1, 3).GetDeterminant();
		float det20 = GetComplementMinor(2, 0).GetDeterminant();
		float det21 = GetComplementMinor(2, 1).GetDeterminant();
		float det22 = GetComplementMinor(2, 2).GetDeterminant();
		float det23 = GetComplementMinor(2, 3).GetDeterminant();
		float det30 = GetComplementMinor(3, 0).GetDeterminant();
		float det31 = GetComplementMinor(3, 1).GetDeterminant();
		float det32 = GetComplementMinor(3, 2).GetDeterminant();
		float det33 = GetComplementMinor(3, 3).GetDeterminant();

		//创建伴随矩阵
		Matrix4x4 matrix{
			det00,-det10,det20,-det30,
			-det01,det11,-det21,det31,
			det02,-det12,det22,-det32,
			-det03,det13,-det23,det33,
		};

		return matrix / det;
	}
	Matrix4x4 Matrix4x4::GetTranspose()
	{
		Matrix4x4 matrix{
			m00,m10,m20,m30,
			m01,m11,m21,m31,
			m02,m12,m22,m32,
			m03,m13,m23,m33
		};

		return matrix;
	}

	Matrix3x2 Matrix4x4::ToMatrix3x2()
	{
		Matrix4x4 matrix = GetTranspose();

		return Matrix3x2(
			matrix.m00, matrix.m01,
			matrix.m10, matrix.m11,
			matrix.m30, matrix.m31
		);
	}

	Vector3 Matrix4x4::MultiplyVector(Vector3 value)
	{
		Vector3 result{};
		result.x = m00 * value.x + m01 * value.y + m02 * value.z;
		result.y = m10 * value.x + m11 * value.y + m12 * value.z;
		result.z = m20 * value.x + m21 * value.y + m22 * value.z;
		return result;
	}

	Vector3 Matrix4x4::MultiplyPoint(Vector3 value)
	{
		Vector3 result{};
		result.x = m00 * value.x + m01 * value.y + m02 * value.z + m03 * 1;
		result.y = m10 * value.x + m11 * value.y + m12 * value.z + m13 * 1;
		result.z = m20 * value.x + m21 * value.y + m22 * value.z + m23 * 1;
		return result;
	}

	Matrix4x4::Matrix4x4()
	{
		m00 = 0; m01 = 0; m02 = 0; m03 = 0;
		m10 = 0; m11 = 0; m12 = 0; m13 = 0;
		m20 = 0; m21 = 0; m22 = 0; m23 = 0;
		m30 = 0; m31 = 0; m32 = 0; m33 = 0;
	}
	Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	{
		this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
		this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
		this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
		this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
	}

	Matrix4x4 Matrix4x4::operator*(Matrix4x4 append)
	{
		Matrix4x4 matrix;
		matrix.m00 = m00 * append.m00 + m01 * append.m10 + m02 * append.m20 + m03 * append.m30;
		matrix.m01 = m00 * append.m01 + m01 * append.m11 + m02 * append.m21 + m03 * append.m31;
		matrix.m02 = m00 * append.m02 + m01 * append.m12 + m02 * append.m22 + m03 * append.m32;
		matrix.m03 = m00 * append.m03 + m01 * append.m13 + m02 * append.m23 + m03 * append.m33;

		matrix.m10 = m10 * append.m00 + m11 * append.m10 + m12 * append.m20 + m13 * append.m30;
		matrix.m11 = m10 * append.m01 + m11 * append.m11 + m12 * append.m21 + m13 * append.m31;
		matrix.m12 = m10 * append.m02 + m11 * append.m12 + m12 * append.m22 + m13 * append.m32;
		matrix.m13 = m10 * append.m03 + m11 * append.m13 + m12 * append.m23 + m13 * append.m33;

		matrix.m20 = m20 * append.m00 + m21 * append.m10 + m22 * append.m20 + m23 * append.m30;
		matrix.m21 = m20 * append.m01 + m21 * append.m11 + m22 * append.m21 + m23 * append.m31;
		matrix.m22 = m20 * append.m02 + m21 * append.m12 + m22 * append.m22 + m23 * append.m32;
		matrix.m23 = m20 * append.m03 + m21 * append.m13 + m22 * append.m23 + m23 * append.m33;

		matrix.m30 = m30 * append.m00 + m31 * append.m10 + m32 * append.m20 + m33 * append.m30;
		matrix.m31 = m30 * append.m01 + m31 * append.m11 + m32 * append.m21 + m33 * append.m31;
		matrix.m32 = m30 * append.m02 + m31 * append.m12 + m32 * append.m22 + m33 * append.m32;
		matrix.m33 = m30 * append.m03 + m31 * append.m13 + m32 * append.m23 + m33 * append.m33;

		return matrix;
	}
	Matrix4x4 Matrix4x4::operator*(float value) {
		Matrix4x4 matrix = *this;
		matrix.m00 *= value; matrix.m01 *= value; matrix.m02 *= value; matrix.m03 *= value;
		matrix.m10 *= value; matrix.m11 *= value; matrix.m12 *= value; matrix.m13 *= value;
		matrix.m20 *= value; matrix.m21 *= value; matrix.m22 *= value; matrix.m23 *= value;
		matrix.m30 *= value; matrix.m31 *= value; matrix.m32 *= value; matrix.m33 *= value;
		return matrix;
	}
	Matrix4x4 Matrix4x4::operator/(float value) {
		return this-> operator* (1 / value);
	}
	Matrix4x4 Matrix4x4::operator*=(Matrix4x4 append)
	{
		return *this = this->operator* (append);
	}


	bool Matrix4x4::operator==(Matrix4x4 append)
	{
		return
			m00 == append.m00 && m01 == append.m01 && m02 == append.m02 && m03 == append.m03 &&
			m10 == append.m10 && m11 == append.m11 && m12 == append.m12 && m13 == append.m13 &&
			m20 == append.m20 && m21 == append.m21 && m22 == append.m22 && m23 == append.m23 &&
			m30 == append.m30 && m31 == append.m31 && m32 == append.m32 && m33 == append.m33;
	}
	bool Matrix4x4::operator!=(Matrix4x4 append)
	{
		return !(*this == append);
	}

	std::wstring Matrix4x4::ToString()
	{
		std::wstringstream stream;
		stream << "{" << std::endl;
		stream << "\t" << m00 << "," << m01 << "," << m02 << "," << m03 << std::endl;
		stream << "\t" << m10 << "," << m11 << "," << m12 << "," << m13 << std::endl;
		stream << "\t" << m20 << "," << m21 << "," << m22 << "," << m23 << std::endl;
		stream << "\t" << m30 << "," << m31 << "," << m32 << "," << m33 << std::endl;
		stream << "}" << std::endl;
		return stream.str();
	}
}
