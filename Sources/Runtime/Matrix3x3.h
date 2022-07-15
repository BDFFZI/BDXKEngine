#pragma once

namespace BDXKEngine {
	struct Matrix3x3
	{
		Matrix3x3() {
			this->m00 = 0; this->m01 = 0; this->m02 = 0;
			this->m10 = 0; this->m11 = 0; this->m12 = 0;
			this->m20 = 0; this->m21 = 0; this->m22 = 0;
		}

		Matrix3x3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
		{
			this->m00 = m00; this->m01 = m01; this->m02 = m02;
			this->m10 = m10; this->m11 = m11; this->m12 = m12;
			this->m20 = m20; this->m21 = m21; this->m22 = m22;
		}

		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;

		float GetDeterminant()
		{
			return m00 * (m11 * m22 - m12 * m21) +
				-m01 * (m10 * m22 - m12 * m20) +
				m02 * (m10 * m21 - m11 * m20);
		}
	};
}

