#pragma once
#include <functional>
#include "Vector2Int.h"

class Screen
{
public:
	static Vector2 GetSize();
protected:
	static void Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent);

private:
	static HWND window;

	static void OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};