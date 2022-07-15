#include "Cursor.h"
#include "Window.h"

namespace BDXKEngine {
	bool Cursor::visible = true;
	HCURSOR Cursor::hCursor = LoadCursor(NULL, IDC_ARROW);
	Window* Cursor::window{};

	void Cursor::SetLockState(bool state)
	{
		window->SetCursorLock(state);
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

	Cursor* Cursor::Initialize(Input* input, Window* window)
	{
		Cursor::window = window;
		window->AddMessageListener(OnWindowMessage);
		return new Cursor();
	}

	void Cursor::UpdateShow()
	{
		if (visible)
			::SetCursor(hCursor);
		else
			::SetCursor(NULL);
	}

	void Cursor::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		switch (messageSign)
		{
		case WM_LBUTTONDOWN:
		{
			SetCapture(window->GetHwnd());
			break;
		}
		case WM_LBUTTONUP:
		{
			if (Input::GetMouseButton(1) == false &&
				Input::GetMouseButton(2) == false)
				ReleaseCapture();
			break;
		}
		case WM_RBUTTONDOWN:
		{
			SetCapture(window->GetHwnd());
			break;
		}
		case WM_RBUTTONUP:
		{
			if (Input::GetMouseButton(0) == false &&
				Input::GetMouseButton(2) == false)
				ReleaseCapture();
			break;
		}
		case WM_MBUTTONDOWN:
		{
			SetCapture(window->GetHwnd());
			break;
		}
		case WM_MBUTTONUP:
		{
			if (Input::GetMouseButton(0) == false &&
				Input::GetMouseButton(1) == false)
				ReleaseCapture();
			break;
		}
		case WM_SETCURSOR:
		{
			if (LOWORD(lparameter) == HTCLIENT)
				Cursor::UpdateShow();
			break;
		}
		}
	}
}