#include "Cursor.h"
#include "Screen.h"

bool Cursor::visible = true;
HCURSOR Cursor::hCursor = LoadCursor(NULL, IDC_HAND);

void Cursor::SetLockState(bool value)
{
	Screen::window->ConfiningCursor(value);
}

void Cursor::SetVisible(bool value)
{
	visible = value;
	Update();
}

void Cursor::SetCursor(wchar_t* value)
{
	hCursor = LoadCursor(NULL, value);
	Update();

	//HCURSOR hCursor = (HCURSOR)LoadImage(
//	NULL,
//	L"C:\\Users\\BDFFZI\\Desktop\\icon.png",
//	IMAGE_CURSOR,
//	0,
//	0,
//	LR_LOADFROMFILE
//	);
}

void Cursor::Update()
{
	if (visible)
		::SetCursor(hCursor);
	else
		::SetCursor(NULL);
}
