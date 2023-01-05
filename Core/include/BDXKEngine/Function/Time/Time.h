#pragma once

namespace BDXKEngine {
	class Time
	{
	public:
		static void Initialize();
		
		static float GetRealtimeSinceStartup();
		static float GetDeltaTime();
		static int GetFrameCount();

		static void RenewFrame();
	private:
		static int frameCount;
		static long startTime;
		static long frameTime;
		static float deltaTime;
		
		static void BeginFrame();
		static void EndFrame();
	};
}

