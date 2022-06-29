#include "Window.h"

Window::Window(const wchar_t* name, MessageHandler messageHandler) :WindowBase(name)
{
	cursorPos = GetCursorLocalPosition();
	lastCursorPos = cursorPos;
	this->messageHandler = messageHandler;
}

void Window::AddMessageListener(MessageListener messageListener)
{
	messageListeners.push_back(messageListener);
}

Rect Window::GetScreenRect()
{
	POINT min{};
	POINT max = GetSize();
	ClientToScreen(hwnd, &min);
	ClientToScreen(hwnd, &max);

	return Rect{ min ,max };
}
Vector2 Window::GetSize()
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return { (float)rect.right,(float)rect.bottom };
}
Vector2 Window::GetCursorLocalPosition()
{
	Rect rect = GetScreenRect();

	POINT point;
	GetCursorPos(&point);

	return (Vector2)point - rect.GetMin();
}
Vector2 Window::GetCursorMoveDelta() {
	return cursorPos - lastCursorPos;
}

void Window::ConfiningCursor(bool isOpen)
{
	if (isOpen)
	{
		RECT rect = GetScreenRect();
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(NULL);
	}
}
void Window::SetCursorLock(bool state)
{
	lockCursorPos = Window::GetCursorLocalPosition();
	lastCursorPos = lockCursorPos;
	cursorlock = state;
}
void Window::SetCursorLocalPosition(Vector2 localPosition)
{
	Rect rect = GetScreenRect();
	Vector2 position = rect.GetMin() + localPosition;

	SetCursorPos((int)(position.x + 0.5f), (int)(position.y + 0.5f));
}

void Window::RePaint(bool clear)
{
	InvalidateRect(hwnd, NULL, clear);
}

LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	LRESULT result = messageHandler(this, messageSign, wparameter, lparameter);
	for (MessageListener listener : messageListeners)
		listener(this, messageSign, wparameter, lparameter);

	switch (messageSign)
	{
	case WM_PAINT:
		if (cursorlock)
			SetCursorLocalPosition({ lockCursorPos.x,lockCursorPos.y });
		else
			lastCursorPos = cursorPos;
		break;
	case WM_MOUSEMOVE:
		cursorPos.x = (float)(lparameter << 48 >> 48);
		cursorPos.y = (float)(lparameter >> 16);
		break;
	case WM_SIZE:
	{
		RePaint();
		break;
	}
	}

	return result;
}

