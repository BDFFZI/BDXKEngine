#include "Input.h"

namespace BDXKEngine {
	Window* Input::window{};
	Vector2 Input::mouseScrollDelta;
	Vector2 Input::mousePosition;
	bool Input::mouseButtonState[3][3];
	bool Input::keyboardState[256][3];

	bool Input::GetMouseButtonDown(int mouseButtonIndex)
	{
		if (mouseButtonState[mouseButtonIndex][0] == false &&
			mouseButtonState[mouseButtonIndex][1] == true)
			return true;
		return false;
	}
	bool Input::GetMouseButton(int mouseButtonIndex)
	{
		if (mouseButtonState[mouseButtonIndex][0] == true &&
			mouseButtonState[mouseButtonIndex][1] == true)
			return true;
		return false;
	}
	bool Input::GetMouseButtonUp(int mouseButtonIndex)
	{
		if (mouseButtonState[mouseButtonIndex][0] == true &&
			mouseButtonState[mouseButtonIndex][1] == false)
			return true;
		return false;
	}

	bool Input::GetKeyDown(KeyCode keyCode)
	{
		if (keyboardState[static_cast<int>(keyCode)][0] == false &&
			keyboardState[static_cast<int>(keyCode)][1] == true)
			return true;
		return false;
	}
	bool Input::GetKey(KeyCode keyCode)
	{
		if (keyboardState[static_cast<int>(keyCode)][0] == true &&
			keyboardState[static_cast<int>(keyCode)][1] == true)
			return true;
		return false;
	}
	bool Input::GetKeyUp(KeyCode keyCode)
	{
		if (keyboardState[static_cast<int>(keyCode)][0] == true &&
			keyboardState[static_cast<int>(keyCode)][1] == false)
			return true;
		return false;
	}

	void Input::Initialize(Window* window)
	{
		Input::window = window;
		window->AddRenewEvent([]() {
			mouseScrollDelta = Vector2::zero;

			for (auto& state : mouseButtonState)
			{
				state[0] = state[1];
				state[1] = state[2];
			}
			for (auto& state : keyboardState)
			{
				state[0] = state[1];
				state[1] = state[2];
			}
			});
		window->AddMouseMoveEvent([](Vector2 position) {
			mousePosition = position;
			});
		window->AddMouseWheelEvent([](Vector2 delta) {
			mouseScrollDelta = delta;
			});
		window->AddMouseButtonEvent([](int mouseButton, bool state) {
			mouseButtonState[mouseButton][2] = state;
			});
		window->AddKeyCodeEvent([](KeyCode keyCode, bool state) {
			keyboardState[static_cast<int>(keyCode)][2] = state;
			});
	}
}