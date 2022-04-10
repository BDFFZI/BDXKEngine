#pragma once

class BDXKEngine;
class Time
{
	friend BDXKEngine;

public:
	static float GetDeltaTime();
	static float GetRealtimeSinceStartup();
private:
	static long startTime;
	static long frameTime;
	static float deltaTime;

	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
};

