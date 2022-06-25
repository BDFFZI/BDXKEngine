#pragma once
#include "Window.h"

class Screen
{
public:
	static Vector2 GetSize() {
		return window->GetSize();
	}
protected:
	static void Initialize(Window* window);
private:
	static Window* window;
};