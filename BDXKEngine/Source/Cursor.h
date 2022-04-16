#pragma once
#include "Window.h"

class BDXKEngine;
class Cursor
{
	friend BDXKEngine;
public:

	static void SetLockState(bool value);
	static void SetVisible(bool value);
	static void SetCursor(wchar_t* value);
private:
	static bool visible;
	static HCURSOR hCursor;

	static void Update();
};

