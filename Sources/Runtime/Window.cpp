#include "Window.h"

namespace BDXKEngine {
	Window::Window(const wchar_t* name) :WindowBase(name)
	{
		cursorPos = GetCursorLocalPosition();
		lastCursorPos = cursorPos;
	}

	void Window::AddRenewEvent(RenewEvent renewEvent)
	{
		renewEvents.push_back(renewEvent);
	}
	void Window::AddResizeEvent(ResizeEvent resizeEvent)
	{
		resizeEvents.push_back(resizeEvent);
	}
	void Window::AddDestroyEvent(DestroyEvent destroyEvent)
	{
		destroyEvents.push_back(destroyEvent);
	}
	void Window::AddMouseMoveEvent(MouseMoveEvent mouseMoveEvent)
	{
		mouseMoveEvents.push_back(mouseMoveEvent);
	}
	void Window::AddMouseWheelEvent(MouseWheelEvent mouseWheelEvent)
	{
		mouseWheelEvents.push_back(mouseWheelEvent);
	}
	void Window::AddMouseButtonEvent(MouseButtonEvent mouseButtonEvent)
	{
		mouseButtonEvents.push_back(mouseButtonEvent);
	}
	void Window::AddKeyCodeEvent(KeyCodeEvent keyCodeEvent)
	{
		keyCodeEvents.push_back(keyCodeEvent);
	}
	void Window::AddCharacterEvent(CharacterEvent characterEvent)
	{
		characterEvents.push_back(characterEvent);
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

	void Window::SetCursorTrack(bool state) {
		if (state)
			SetCapture(hwnd);
		else
			ReleaseCapture();
	}
	void Window::SetCursorConfining(bool isOpen)
	{
		if (isOpen)
		{
			RECT rect = GetScreenRect();
			ClipCursor(&rect);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}
	void Window::SetCursorLock(bool state)
	{
		lockCursorPos = Window::GetCursorLocalPosition();
		lastCursorPos = lockCursorPos;
		cursorlock = state;
	}
	void Window::SetCursorVisible(bool state)
	{
		cursorVisible = state;
		UpdateCursor();
	}


	void Window::SetCursorLocalPosition(Vector2 localPosition)
	{
		Rect rect = GetScreenRect();
		Vector2 position = rect.GetMin() + localPosition;

		SetCursorPos((int)(position.x + 0.5f), (int)(position.y + 0.5f));
	}

	void Window::UpdateCursor()
	{
		if (cursorVisible)
			SetCursor(hCursor);
		else
			SetCursor(nullptr);
	}

	LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		switch (messageSign)
		{
		case WM_PAINT:
		{
			for (auto& renewEvent : renewEvents)
				renewEvent();

			//为了让用户能获取到鼠标增量，放后执行
			if (cursorlock)
				SetCursorLocalPosition({ lockCursorPos.x,lockCursorPos.y });
			else
				lastCursorPos = cursorPos;
			break;
		}
		case WM_MOUSEMOVE:
		{
			cursorPos.x = (float)(lparameter & 0xffff);
			cursorPos.y = (float)(lparameter >> 16);
			for (auto& mouseMoveEvent : mouseMoveEvents)
				mouseMoveEvent(cursorPos);
			break;
		}
		case WM_CHAR:
		{
			wchar_t character = (wchar_t)wparameter;
			for (auto& characterEvent : characterEvents)
				characterEvent(character);
			break;
		}
		case WM_KEYDOWN:
		{
			KeyCode keyCode = (KeyCode)wparameter;
			for (auto& keyCodeEvent : keyCodeEvents)
				keyCodeEvent(keyCode, true);
			break;
		}
		case WM_KEYUP:
		{
			KeyCode keyCode = (KeyCode)wparameter;
			for (auto& keyCodeEvent : keyCodeEvents)
				keyCodeEvent(keyCode, false);
			break;
		}
#pragma region 鼠标事件（除了移动鼠标事件）
		case WM_LBUTTONDOWN:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(0, true);
			break;
		}
		case WM_LBUTTONUP:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(0, false);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(1, true);
			break;
		}
		case WM_RBUTTONUP:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(1, false);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			Vector2 delta = { 0,GET_WHEEL_DELTA_WPARAM(wparameter) / 120.0f };
			for (auto& mouseWheelEvent : mouseWheelEvents)
				mouseWheelEvent(delta);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(2, true);
			break;
		}
		case WM_MBUTTONUP:
		{
			for (auto& mouseButtonEvent : mouseButtonEvents)
				mouseButtonEvent(2, false);
			break;
		}
#pragma endregion
		case WM_SETCURSOR:
		{
			if (LOWORD(lparameter) == HTCLIENT)
				UpdateCursor();
			break;
		}
		case WM_SIZE:
		{
			InvalidateRect(hwnd, nullptr, true);

			Vector2 size = {
				(float)(lparameter & 0xffff),
				(float)(lparameter >> 16)
			};
			for (auto& resizeEvent : resizeEvents)
				resizeEvent(size);
			break;
		}
		case WM_DESTROY:
		{
			for (auto& destroyEvent : destroyEvents)
				destroyEvent();
			break;
		}
		}

		return DefWindowProcW(hwnd, messageSign, wparameter, lparameter);
	}
}
