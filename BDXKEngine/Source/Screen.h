#pragma once
#include "Window.h"

class Screen
{
protected:
	static void Initialize(Window* window);
private:
	static Window* window;
};