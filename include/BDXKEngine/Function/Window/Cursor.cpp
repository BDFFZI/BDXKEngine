#include "Cursor.h"

namespace BDXKEngine {
	Window* Cursor::window = nullptr;
	int Cursor::mouseButtonTime = 0;

	void Cursor::SetLockState(bool state)
	{
		window->SetCursorLock(state);
	}

	void Cursor::SetVisible(bool state)
	{
		window->SetCursorVisible(state);
	}

	void Cursor::Initialize(Window* window)
	{
		Cursor::window = window;
		window->AddMouseButtonEvent([=](int buttonButton, bool state) {
			if (state)
				mouseButtonTime++;
			else
				mouseButtonTime--;

			window->SetCursorTrack(mouseButtonTime != 0);
			});
	}
}