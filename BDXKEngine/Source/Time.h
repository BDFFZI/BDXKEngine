#pragma once
#include <chrono>
#include <functional>
#include <Windows.h>

class Time
{
public:
	static float GetDeltaTime();
	static float GetRealtimeSinceStartup();
protected:
	static void Initialize(std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent);
private:
	static long startTime;
	static long frameTime;
	static float deltaTime;

	static void BeginFrame();
	static void EndFrame();
	static void OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

