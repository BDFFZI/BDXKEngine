#pragma once
#include <functional>
#include "Window.h"
#include "Input.h"

class Cursor
{
public:
	static void SetLockState(bool value);
	static void SetVisible(bool value);
	static void SetCursor(wchar_t* value);
protected:
	static Cursor* Initialize(Input* input, Window* window);
private:
	static bool visible;
	static HCURSOR hCursor;
	static Window* window;

	static void UpdateShow();

	static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

