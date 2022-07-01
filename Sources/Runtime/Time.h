#pragma once
#include <chrono>
#include <functional>
#include <Windows.h>
#include "Window.h"

class Time
{
public:
	static float GetDeltaTime();
	static float GetRealtimeSinceStartup();
protected:
	static void Initialize(Window* window);
private:
	static long startTime;
	static long frameTime;
	static float deltaTime;

	static void BeginFrame();
	static void EndFrame();
	static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

