#include "Input.h"

//const int KeyCode::Backspace = 0x08;
//const int KeyCode::Delete = 0x2E;
//const int KeyCode::Tab = 0x09;
//const int KeyCode::Return = 0x0D;
//const int KeyCode::Escape = 0x1B;
//const int KeyCode::Space = 0x20;
//const int KeyCode::Alpha0 = 0x30;
//const int KeyCode::Alpha1 = 0x31;
//const int KeyCode::Alpha2 = 0x32;
//const int KeyCode::Alpha3 = 0x33;
//const int KeyCode::Alpha4 = 0x34;
//const int KeyCode::Alpha5 = 0x35;
//const int KeyCode::Alpha6 = 0x36;
//const int KeyCode::Alpha7 = 0x37;
//const int KeyCode::Alpha8 = 0x38;
//const int KeyCode::Alpha9 = 0x39;
//const int KeyCode::A = 0x41;
//const int KeyCode::B = 0x42;
//const int KeyCode::C = 0x43;
//const int KeyCode::D = 0x44;
//const int KeyCode::E = 0x45;
//const int KeyCode::F = 0x46;
//const int KeyCode::G = 0x47;
//const int KeyCode::H = 0x48;
//const int KeyCode::I = 0x49;
//const int KeyCode::J = 0x4A;
//const int KeyCode::K = 0x4B;
//const int KeyCode::L = 0x4C;
//const int KeyCode::M = 0x4D;
//const int KeyCode::N = 0x4E;
//const int KeyCode::O = 0x4F;
//const int KeyCode::P = 0x50;
//const int KeyCode::Q = 0x51;
//const int KeyCode::R = 0x52;
//const int KeyCode::S = 0x53;
//const int KeyCode::T = 0x54;
//const int KeyCode::U = 0x55;
//const int KeyCode::V = 0x56;
//const int KeyCode::W = 0x57;
//const int KeyCode::X = 0x58;
//const int KeyCode::Y = 0x59;
//const int KeyCode::Z = 0x5A;
//const int KeyCode::RightShift = 0xA0;
//const int KeyCode::LeftShift = 0xA1;

Vector2 Input::mousePosition;
float Input::mouseScrollDelta;
bool Input::lastMouseButtonState[3];
bool Input::mouseButtonState[3];
bool Input::lastKeyboardState[256];
bool Input::keyboardState[256];

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

bool Input::GetKeyDown(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == false &&
		keyboardState[(int)keyCode] == true)
		return true;
	return false;
}

bool Input::GetKey(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == true &&
		keyboardState[(int)keyCode] == true)
		return true;
	return false;
}

bool Input::GetKeyUp(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == true &&
		keyboardState[(int)keyCode] == false)
		return true;
	return false;
}

void Input::FlushState()
{
	for (int i = 0; i < 3; i++)
	{
		lastMouseButtonState[i] = mouseButtonState[i];
	}
	mouseScrollDelta *= 0.7f;

	for (int i = 0; i < 256; i++)
	{
		lastKeyboardState[i] = keyboardState[i];
	}
}
