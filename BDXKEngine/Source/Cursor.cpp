#include "Cursor.h"

bool Cursor::visible = true;
HCURSOR Cursor::hCursor = LoadCursor(NULL, IDC_HAND);
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

	//HCURSOR hCursor = (HCURSOR)LoadImage(
//	NULL,
//	L"C:\\Users\\BDFFZI\\Desktop\\icon.png",
//	IMAGE_CURSOR,
//	0,
//	0,
//	LR_LOADFROMFILE
//	);
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
