#pragma once
#include <chrono>
#include "Window.h"

namespace BDXKEngine {
	class Time
	{
	public:
		static float GetRealtimeSinceStartup();
		static float GetDeltaTime();
		static float GetFrameCount();
	protected:
		static void Initialize(Window* window);
	private:
		static int frameCount;
		static long startTime;
		static long frameTime;
		static float deltaTime;

		static void BeginFrame();
		static void EndFrame();
	};
}

