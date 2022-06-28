#include "Window.h"

Rect Window::GetScreenRect(HWND hwnd)
{
	POINT min{};
	POINT max = GetSize(hwnd);
	ClientToScreen(hwnd, &min);
	ClientToScreen(hwnd, &max);

	return Rect{ min ,max };
}
Vector2 Window::GetSize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return { (float)rect.right,(float)rect.bottom };
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

Vector2 Window::GetCursorLocalPosition(HWND hwnd)
{
	Rect rect = GetScreenRect(hwnd);

	POINT point;
	GetCursorPos(&point);

	return (Vector2)point - rect.GetMin();
}
void Window::SetCursorLocalPosition(HWND hwnd, Vector2 localPosition)
{
	Rect rect = GetScreenRect(hwnd);
	Vector2 position = rect.GetMin() + localPosition;

	SetCursorPos((int)(position.x + 0.5f), (int)(position.y + 0.5f));
}
Vector2 Window::GetCursorMoveDelta() {
	return cursorPos - lastCursorPos;
}
void Window::SetCursorLock(HWND hwnd, bool state)
{
	lockCursorPos = Window::GetCursorLocalPosition(hwnd);
	lastCursorPos = lockCursorPos;
	cursorlock = state;
}

Window::Window(const wchar_t* name,
	std::function<LRESULT(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
) :WindowBase(name)
{
	cursorPos = GetCursorLocalPosition(hwnd);
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
			SetCursorLocalPosition(hwnd, { lockCursorPos.x,lockCursorPos.y });
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

