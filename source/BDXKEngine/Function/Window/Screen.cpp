#include "Screen.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	Vector2 Screen::GetSize()
	{
		return Window::GetSize();
	}
}
