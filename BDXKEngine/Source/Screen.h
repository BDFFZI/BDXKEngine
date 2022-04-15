#pragma once
#include "Vector2Int.h"
#include "String.h"
#include "Window.h"

class BDXKEngine;
class Cursor;
class Screen
{
	friend BDXKEngine;
	friend Cursor;
public:

private:
	static Window* window;

	static void Initialize(Window* window);
};