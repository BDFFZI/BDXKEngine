#pragma once
#include <random>
#include <ctime>
#include "Base/Color.h"
#include "Base/Vector/Vector3.h"

namespace BDXKEngine {
	class Random {
	public:
		static Color ColorHSV()
		{
			seed++;

			std::default_random_engine engine;
			engine.seed(seed);
			std::uniform_int_distribution<> random(0, 255);
			return Color{
				random(engine) / 255.0f,
				random(engine) / 255.0f ,
				random(engine) / 255.0f ,
				random(engine) / 255.0f
			};
		}

		static Vector3 InsideUnitSphere()
		{
			return {};
		}

		static Vector3 OnUnitSphere()
		{
			return {};
		}

		inline static int seed = time(nullptr);
	};
}