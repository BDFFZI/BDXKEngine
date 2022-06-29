#include "Screen.h"
#include "Window.h"

Vector2 Screen::GetSize()
{
	return window->GetSize();
}

void Screen::Initialize(Window* window)
{
	Screen::window = window;
}