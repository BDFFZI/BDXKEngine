#include "Window.h"

Rect Window::GetRect(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return rect;
}
Rect Window::GetScreenRect(HWND hwnd)
{
	Rect rect = GetRect(hwnd);
	POINT min = rect.GetMin();
	POINT max = rect.GetMax();
	ClientToScreen(hwnd, &min);
	ClientToScreen(hwnd, &max);

	return Rect{ min ,max };
}
Vector2 Window::GetSize(HWND hwnd)
{
	return GetRect(hwnd).GetSize();
}
void Window::ConfiningCursor(HWND hwnd, bool isOpen)
{
	if (isOpen)
	{
		RECT rect = GetScreenRect(hwnd);
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(NULL);
	}
}
void Window::RePaint(HWND hwnd, bool clear)
{
	InvalidateRect(hwnd, NULL, clear);
}

Vector2 Window::GetCursorPos()
{
	POINT point;
	::GetCursorPos(&point);
	return point;
}
Vector2 Window::GetCursorMoveDelta() {
	return cursorPos - lastCursorPos;
}
void Window::SetCursorLock(bool state)
{
	lockCursorPos = Window::GetCursorPos();
	lastCursorPos = cursorPos;
	cursorlock = state;
}

Window::Window(const wchar_t* name,
	std::function<LRESULT(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
) :WindowBase(name)
{
	cursorPos = GetCursorPos();
	lastCursorPos = cursorPos;
	this->messageEvent = messageEvent;
}


LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	LRESULT result = messageEvent(hwnd, messageSign, wparameter, lparameter);

	switch (messageSign)
	{
	case WM_PAINT:
		if (cursorlock)
			SetCursorPos((int)(lockCursorPos.x + 0.5f), (int)(lockCursorPos.y + 0.5f));
		else
			lastCursorPos = cursorPos;
		break;
	case WM_MOUSEMOVE:
		cursorPos.x = (float)(lparameter << 48 >> 48);
		cursorPos.y = (float)(lparameter >> 16);
		break;
	}

	return result;
}

