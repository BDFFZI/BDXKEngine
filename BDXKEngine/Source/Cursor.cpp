#include "Cursor.h"
#include "Window.h"

bool Cursor::visible = true;
HCURSOR Cursor::hCursor = LoadCursor(NULL, IDC_ARROW);
HWND Cursor::window{};

void Cursor::SetLockState(bool state)
{
	Window::SetCursorLock(state);
}

void Cursor::SetVisible(bool value)
{
	visible = value;
	UpdateShow();
}

void Cursor::SetCursor(wchar_t* value)
{
	hCursor = LoadCursor(NULL, value);
	UpdateShow();
}

void Cursor::Initialize(HWND window, std::function<void(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>* windowEvent)
{
	Cursor::window = window;
	*windowEvent = OnWindowMessage;
}

void Cursor::UpdateShow()
{
	if (visible)
		::SetCursor(hCursor);
	else
		::SetCursor(NULL);
}

void Cursor::OnWindowMessage(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_SETCURSOR:
	{
		if (LOWORD(lparameter) == HTCLIENT)
			Cursor::UpdateShow();
		else
			DefWindowProc(window, messageSign, wparameter, lparameter);
		break;
	}
	}
}
