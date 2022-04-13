#pragma once
#include "Vector2Int.h"
#include "String.h"

class WindowUtility
{
public:
	static Vector2Int GetWindowSize(HWND hwnd);
	static String GetOpenFilePath();
};