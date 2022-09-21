#include "Input.h"
#include "Base/Extension/Debug.h"

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
		if (keyboardState[(int)keyCode][0] == false &&
			keyboardState[(int)keyCode][1] == true)
			return true;
		return false;
	}
	bool Input::GetKey(KeyCode keyCode)
	{
		if (keyboardState[(int)keyCode][0] == true &&
			keyboardState[(int)keyCode][1] == true)
			return true;
		return false;
	}
	bool Input::GetKeyUp(KeyCode keyCode)
	{
		if (keyboardState[(int)keyCode][0] == true &&
			keyboardState[(int)keyCode][1] == false)
			return true;
		return false;
	}

	void Input::Initialize(Window* window)
	{
		Input::window = window;
		window->AddRenewEvent([]() {
			mouseScrollDelta = Vector2::zero;

			for (int i = 0; i < 3; i++)
			{
				mouseButtonState[i][0] = mouseButtonState[i][1];
				mouseButtonState[i][1] = mouseButtonState[i][2];
			}
			for (int i = 0; i < 256; i++)
			{
				keyboardState[i][0] = keyboardState[i][1];
				keyboardState[i][1] = keyboardState[i][2];
			}
			});
		window->AddMouseMoveEvent([](Vector2 position) {
			Input::mousePosition = position;
			});
		window->AddMouseWheelEvent([](Vector2 delta) {
			Input::mouseScrollDelta = delta;
			});
		window->AddMouseButtonEvent([](int mouseButton, bool state) {
			Input::mouseButtonState[mouseButton][2] = state;
			});
		window->AddKeyCodeEvent([](KeyCode keyCode, bool state) {
			Input::keyboardState[(int)keyCode][2] = state;
			});
	}
}