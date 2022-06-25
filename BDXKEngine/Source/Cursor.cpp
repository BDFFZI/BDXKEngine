#include "Cursor.h"

bool Cursor::visible = true;
HCURSOR Cursor::hCursor = LoadCursor(NULL, IDC_ARROW);
Window* Cursor::window = NULL;

void Cursor::SetLockState(bool value)
{
	window->ConfiningCursor(value);
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

void Cursor::Initialize(Window* window)
{
	Cursor::window = window;
}

void Cursor::UpdateShow()
{
	if (visible)
		::SetCursor(hCursor);
	else
		::SetCursor(NULL);
}
