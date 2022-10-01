#pragma once
#include "Vector3.h"

namespace BDXKEngine {
	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;

		float operator[](int index) {

		}

		Vector4(Vector3 vector3, float w)
		{
			x = vector3.x;
			y = vector3.y;
			z = vector3.z;
			this->w = w;
		}

		Vector4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
	};
}

