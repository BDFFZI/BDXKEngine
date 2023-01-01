#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	class Time
	{
	public:
		static void Initialize(Window* window);
		
		static float GetRealtimeSinceStartup();
		static float GetDeltaTime();
		static int GetFrameCount();
	private:
		static int frameCount;
		static long startTime;
		static long frameTime;
		static float deltaTime;
		

		static void BeginFrame();
		static void EndFrame();
	};
}

