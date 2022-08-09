#include "Cursor.h"

namespace BDXKEngine {
	bool Cursor::visible = true;
	Window* Cursor::window{};

	void Cursor::SetLockState(bool state)
	{
		window->SetCursorLock(state);
	}

	void Cursor::SetVisible(bool value)
	{
		visible = value;
		window->SetCursorVisible(value);
	}

	Cursor* Cursor::Initialize(Input* input, Window* window)
	{
		Cursor::window = window;
		return nullptr;
	}
}