#pragma once
#include "Window.h"

class Cursor
{
public:
	static void SetLockState(bool value);
	static void SetVisible(bool value);
	static void SetCursor(wchar_t* value);
protected:
	static void Initialize(Window* window);
	static void UpdateShow();
private:
	static bool visible;
	static HCURSOR hCursor;
	static Window* window;
};

