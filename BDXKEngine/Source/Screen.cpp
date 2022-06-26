#include "Screen.h"
#include "Window.h"

HWND Screen::window{};



Vector2 Screen::GetSize()
{
	return Window::GetSize(window);;
}

void Screen::Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent)
{
	Screen::window = window;
	*windowEvent = OnWindowMessage;
}

void Screen::OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{

}