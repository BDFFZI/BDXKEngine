#pragma once
#include <functional>
#include <Windows.h>
#include "Vector2.h"

class Cursor
{
public:
	static void SetLockState(bool value);
	static void SetVisible(bool value);
	static void SetCursor(wchar_t* value);
protected:
	static void Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent);
private:
	static bool visible;
	static HCURSOR hCursor;
	static HWND window;

	static void UpdateShow();

	static void OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

