#include "Cursor.h"
#include "Screen.h"

void Cursor::SetLockState(bool value)
{
	Screen::window->ConfiningCursor(value);
}
