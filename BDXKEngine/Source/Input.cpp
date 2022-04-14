#include "Input.h"

Vector2 Input::mousePosition;
bool Input::lastMouseButtonState[3];
bool Input::mouseButtonState[3];
bool Input::lastKeyboardState[26];
bool Input::keyboardState[26];

bool Input::GetMouseButtonDown(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == false &&
		mouseButtonState[mouseButtonIndex] == true)
		return true;
	return false;
}

bool Input::GetMouseButton(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == true &&
		mouseButtonState[mouseButtonIndex] == true)
		return true;
	return false;
}

bool Input::GetMouseButtonUp(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == true &&
		mouseButtonState[mouseButtonIndex] == false)
		return true;
	return false;
}

void Input::Update()
{
	for (int i = 0; i < 3; i++)
	{
		lastMouseButtonState[i] = mouseButtonState[i];
	}
}
