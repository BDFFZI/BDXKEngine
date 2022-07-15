#include "Screen.h"
#include "Window.h"

namespace BDXKEngine {
	Vector2 Screen::GetSize()
	{
		return window->GetSize();
	}

	void Screen::Initialize(Window* window)
	{
		Screen::window = window;
	}
}